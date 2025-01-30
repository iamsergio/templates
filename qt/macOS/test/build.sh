unset SDKROOT

qt-cmake -G Ninja --preset=rel && cd build-rel && \
ninja -j10  && \
macdeployqt test.app/ -qmldir=../ -appstore-compliant -sign-for-notarization=$APPLE_DEV_ID_APPLICATION && \
codesign --verify --deep --verbose=1 test.app
