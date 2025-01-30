unset SDKROOT

mkdir build-macos-release-xcode &> /dev/null

cd build-macos-release-xcode && \
qt-cmake -G "Xcode" -DCMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM=$APPLE_TEAM_ID .. && \
xcodebuild -scheme test -configuration Release && \
macdeployqt Release/test.app/ -qmldir=../ -codesign=$APPLE_DEV_ID_APPLICATION
