#include <print>
#include <utility>

struct WithCopyAssign
{
    WithCopyAssign(int v)
        : value(v)
    {
        std::println("CTOR WithCopyAssign");
    }

    WithCopyAssign()
    {
        std::println("CTOR WithCopyAssign");
    }

    ~WithCopyAssign()
    {
        std::println("DTOR WithCopyAssign");
    }

    WithCopyAssign &operator=(const WithCopyAssign &other)
    {
        WithCopyAssign tmp = other;
        tmp.swap(*this);
        return *this;
    }

    void swap(WithCopyAssign &other) noexcept
    {
        std::swap(this->value, other.value);
    }

    int value = 42;
};

struct WithUnifiedAssign
{
    WithUnifiedAssign(int v)
        : value(v)
    {
    }

    WithUnifiedAssign()
    {
        std::println("CTOR WithUnifiedAssign");
    }

    ~WithUnifiedAssign()
    {
        std::println("DTOR WithUnifiedAssign");
    }

    WithUnifiedAssign &operator=(WithUnifiedAssign other)
    {
        other.swap(*this);
        return *this;
    }

    void swap(WithUnifiedAssign &other) noexcept
    {
        std::swap(this->value, other.value);
    }

    int value = 42;
};


int main()
{
    WithCopyAssign w = { 1 };
    WithCopyAssign w2 = { 2 };

    w = w2;
    w = WithCopyAssign(1);
    std::println("w={}, w2={}", w.value, w2.value);

    WithUnifiedAssign u = { 1 };
    WithUnifiedAssign u2 = { 2 };

    u = u2;
    u = WithUnifiedAssign();

    return 0;
}
