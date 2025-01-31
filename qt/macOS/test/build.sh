unset SDKROOT

rm -rf build-rel &> /dev/null

qt-cmake -G Ninja --preset=rel && cd build-rel && \
ninja -j10  && \
macdeployqt test.app/ -qmldir=../ -appstore-compliant -hardened-runtime -codesign=$APPLE_DEV_ID_APPLICATION && \
codesign --verify --deep --verbose=1 test.app
