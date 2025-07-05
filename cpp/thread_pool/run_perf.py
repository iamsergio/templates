#!/usr/bin/env python3

import subprocess
import sys
import os
import re

def parse_perf_output(output):
    """Parse perf stat output and extract key metrics."""
    metrics = {}
    
    # Patterns to match perf output lines
    patterns = {
        'time_elapsed': r'([\d,]+\.?\d*)\s+seconds time elapsed',
        'task_clock': r'([\d,]+\.?\d*)\s+msec task-clock',
        'context_switches': r'(\d+)\s+context-switches',
        'cpu_migrations': r'(\d+)\s+cpu-migrations',
        'page_faults': r'([\d,]+)\s+page-faults',
        'instructions': r'([\d,]+)\s+instructions',
        'cycles': r'([\d,]+)\s+cycles',
        'stalled_cycles_frontend': r'([\d,]+)\s+stalled-cycles-frontend',
        'branches': r'([\d,]+)\s+branches',
        'branch_misses': r'([\d,]+)\s+branch-misses'
    }
    
    for key, pattern in patterns.items():
        match = re.search(pattern, output)
        if match:
            value_str = match.group(1).replace(',', '')
            try:
                if key == 'time_elapsed' or key == 'task_clock':
                    metrics[key] = float(value_str)
                else:
                    metrics[key] = int(value_str)
            except ValueError:
                metrics[key] = 0
        else:
            metrics[key] = 0
    
    return metrics

def run_perf_stat(command, description, env=None):
    """Run perf stat with the given command and return parsed results."""
    print(f"Running: {description}...")
    
    try:
        # Use custom environment if provided, otherwise use current environment
        cmd_env = env if env is not None else os.environ.copy()
        
        result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, env=cmd_env)
        if result.returncode != 0:
            print(f"Warning: Command exited with code {result.returncode}")
            return None
        
        # perf stat outputs to stderr, but we captured both
        output = result.stdout + result.stderr
        return parse_perf_output(output)
        
    except FileNotFoundError:
        print("Error: perf command not found. Make sure perf is installed.")
        return None
    except Exception as e:
        print(f"Error running command: {e}")
        return None

def format_number(num):
    """Format large numbers with appropriate units."""
    if num >= 1_000_000_000:
        return f"{num/1_000_000_000:.2f}B"
    elif num >= 1_000_000:
        return f"{num/1_000_000:.2f}M"
    elif num >= 1_000:
        return f"{num/1_000:.2f}K"
    else:
        return f"{num:.2f}"

def calculate_improvement(base_value, current_value):
    """Calculate percentage improvement (negative means better performance)."""
    if base_value == 0:
        return 0
    return ((current_value - base_value) / base_value) * 100

def print_results_table(results):
    """Print results in a formatted table with percentage improvements."""
    if not results or len(results) < 2:
        print("Error: Need at least 2 benchmark results for comparison")
        return
    
    configurations = list(results.keys())
    base_config = configurations[0]
    base_results = results[base_config]
    
    # Table headers
    print(f"\n{'='*80}")
    print("PERFORMANCE COMPARISON TABLE")
    print(f"{'='*80}")
    
    # Header row
    header = f"{'Metric':<25}"
    for config in configurations:
        header += f"{config:<25}"
    print(header)
    print("-" * 80)
    
    # Metrics to display
    metrics_info = [
        ('time_elapsed', 'Time (seconds)', lambda x: f"{x:.3f}"),
        ('task_clock', 'Task Clock (ms)', lambda x: f"{x:.2f}"),
        ('cycles', 'Cycles', format_number),
        ('instructions', 'Instructions', format_number),
        ('branches', 'Branches', format_number),
        ('branch_misses', 'Branch Misses', format_number),
        ('page_faults', 'Page Faults', format_number),
        ('context_switches', 'Context Switches', lambda x: f"{x}"),
        ('cpu_migrations', 'CPU Migrations', lambda x: f"{x}")
    ]
    
    for metric_key, metric_name, formatter in metrics_info:
        row = f"{metric_name:<25}"
        
        for i, config in enumerate(configurations):
            if metric_key in results[config]:
                value = results[config][metric_key]
                formatted_value = formatter(value)
                
                if i == 0:  # Base configuration
                    row += f"{formatted_value:<25}"
                else:  # Show improvement percentage
                    base_value = base_results.get(metric_key, 0)
                    improvement = calculate_improvement(base_value, value)
                    
                    if improvement < 0:
                        row += f"{formatted_value} ({improvement:.1f}%)"[:24].ljust(25)
                    elif improvement > 0:
                        row += f"{formatted_value} (+{improvement:.1f}%)"[:24].ljust(25)
                    else:
                        row += f"{formatted_value} (0.0%)"[:24].ljust(25)
            else:
                row += f"N/A{'':<22}"
        
        print(row)
    
    print("-" * 80)
    
    # Summary
    print("\nSUMMARY:")
    if len(configurations) >= 2:
        base_time = base_results.get('time_elapsed', 0)
        second_time = results[configurations[1]].get('time_elapsed', 0)
        if base_time > 0 and second_time > 0:
            time_improvement = calculate_improvement(base_time, second_time)
            if time_improvement < 0:
                print(f"• {configurations[1]} is {abs(time_improvement):.1f}% faster than {configurations[0]}")
            else:
                print(f"• {configurations[1]} is {time_improvement:.1f}% slower than {configurations[0]}")

def check_executable():
    """Check if the benchmark executable exists."""
    if not os.path.exists('./bench'):
        print("Error: './bench' executable not found.")
        print("Please compile the benchmark first using CMake.")
        return False
    return True

def check_allocator_library(lib_path):
    """Check if an allocator library exists."""
    return os.path.exists(lib_path)

def find_allocator_library(lib_name):
    """Find allocator library in common locations."""
    common_paths = [
        f"/usr/lib/lib{lib_name}.so",
        f"/usr/lib/x86_64-linux-gnu/lib{lib_name}.so",
        f"/usr/lib64/lib{lib_name}.so",
        f"/usr/local/lib/lib{lib_name}.so"
    ]
    
    # Special handling for specific libraries
    if lib_name == "jemalloc":
        common_paths.extend([
            "/usr/lib/x86_64-linux-gnu/libjemalloc.so.2",
            "/usr/lib64/libjemalloc.so.2",
            "/usr/lib/libjemalloc.so.2"
        ])
    elif lib_name == "tcmalloc":
        common_paths.extend([
            "/usr/lib/x86_64-linux-gnu/libtcmalloc.so.4",
            "/usr/lib64/libtcmalloc.so.4",
            "/usr/lib/libtcmalloc.so.4",
            "/usr/lib/x86_64-linux-gnu/libtcmalloc_minimal.so.4",
            "/usr/lib64/libtcmalloc_minimal.so.4"
        ])
    elif lib_name == "mimalloc":
        common_paths.extend([
            "/usr/lib/x86_64-linux-gnu/libmimalloc.so.2.0",
            "/usr/lib64/libmimalloc.so.2.0",
            "/usr/lib/libmimalloc.so.2.0",
            "/usr/lib/x86_64-linux-gnu/libmimalloc.so.2",
            "/usr/lib64/libmimalloc.so.2"
        ])
    
    for path in common_paths:
        if os.path.exists(path):
            return path
    
    return None

def main():
    print("Performance Analysis of Memory Benchmark")
    print("This script runs perf stat on the benchmark with different configurations")
    
    if not check_executable():
        sys.exit(1)
    
    # Check which allocator libraries are available
    allocator_names = ['jemalloc', 'mimalloc', 'tcmalloc']
    
    available_libs = {}
    for name in allocator_names:
        path = find_allocator_library(name)
        if path:
            available_libs[name] = path
            print(f"Found {name} at {path}")
        else:
            print(f"Warning: {name} not found - skipping")
    
    # Test configurations
    configurations = [
        {
            'name': 'Glibc',
            'command': ['perf', 'stat', './bench', '--glibc'],
            'description': 'Benchmark with glibc',
            'env': None
        },
        {
            'name': 'Glibc',
            'command': ['perf', 'stat', './bench'],
            'description': 'Benchmark with glibc',
            'env': None
        },
        {
            'name': 'Glibc No Free',
            'command': ['perf', 'stat', './bench', '--no-free'],
            'description': 'Benchmark with glibc (without memory free)',
            'env': None
        },
        {
            'name': 'Boost',
            'command': ['perf', 'stat', './bench', '--boost'],
            'description': 'Benchmark with Boost memory pool',
            'env': None
        }
    ]
    
    # Add jemalloc configurations if available
    if 'jemalloc' in available_libs:
        jemalloc_env = os.environ.copy()
        jemalloc_env['LD_PRELOAD'] = available_libs['jemalloc']
        
        configurations.extend([
            {
                'name': 'Jemalloc',
                'command': ['perf', 'stat', './bench'],
                'description': 'Benchmark with jemalloc',
                'env': jemalloc_env
            }
        ])
    
    # Add mimalloc configurations if available
    if 'mimalloc' in available_libs:
        mimalloc_env = os.environ.copy()
        mimalloc_env['LD_PRELOAD'] = available_libs['mimalloc']
        
        configurations.extend([
            {
                'name': 'Mimalloc',
                'command': ['perf', 'stat', './bench'],
                'description': 'Benchmark with mimalloc',
                'env': mimalloc_env
            },
            {
                'name': 'Boost+Mimalloc',
                'command': ['perf', 'stat', './bench', '--boost'],
                'description': 'Benchmark with Boost memory pool and mimalloc',
                'env': mimalloc_env
            }
        ])
    
    # Add tcmalloc configurations if available
    if 'tcmalloc' in available_libs:
        tcmalloc_env = os.environ.copy()
        tcmalloc_env['LD_PRELOAD'] = available_libs['tcmalloc']
        
        configurations.extend([
            {
                'name': 'TCMalloc',
                'command': ['perf', 'stat', './bench'],
                'description': 'Benchmark with tcmalloc',
                'env': tcmalloc_env
            }
        ])
    
    results = {}
    
    for config in configurations:
        result = run_perf_stat(config['command'], config['description'], config.get('env'))
        if result:
            results[config['name']] = result
        else:
            print(f"Failed to run: {config['description']}")
    
    if len(results) >= 2:
        print_results_table(results)
    else:
        print("\nError: Not enough successful benchmark runs for comparison")
        print("\nNote: Common issues:")
        print("- perf might require sudo privileges")
        print("- The benchmark executable might not exist")
        print("- Performance counters might not be available")
        print("- Allocator libraries might not be installed")

if __name__ == "__main__":
    main()
