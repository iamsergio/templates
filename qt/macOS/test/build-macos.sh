# Tests building with both Ninja and Xcode

unset SDKROOT

mkdir build-macos-release &> /dev/null
mkdir build-macos-release-xcode &> /dev/null

cd build-macos-release && \
qt-cmake -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_PREFIX=installed .. && \
ninja -j10 && ninja install && \
macdeployqt installed/test.app/ -qmldir=../ -codesign=$APPLE_DEVELOPMENT_IDENTITY && \
cd ../build-macos-release-xcode && \
qt-cmake -G "Xcode" .. && \
xcodebuild -scheme test -configuration Release

