unset SDKROOT

# Required via SSH
# security unlock-keychain

mkdir build-debug-ios-xcode &> /dev/null

cd build-debug-ios-xcode && \
qt-cmake -G Xcode -DCMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM=$APPLE_TEAM_ID .. && \
xcodebuild -allowProvisioningUpdates -scheme test -configuration Debug -destination "name=iPhone 15" -destination-timeout 1 ENABLE_ONLY_ACTIVE_RESOURCES=NO

cd .. && cd build-release-ios-xcode && \
qt-cmake -G Xcode -DCMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM=$APPLE_TEAM_ID .. && \
xcodebuild -allowProvisioningUpdates -scheme test -configuration Release -destination "generic/platform=iOS" -destination-timeout 1 ENABLE_ONLY_ACTIVE_RESOURCES=NO


# For simulator:
# xcodebuild -allowProvisioningUpdates -scheme test -configuration Debug -destination "generic/platform=iOS" -destination-timeout 1 ENABLE_ONLY_ACTIVE_RESOURCES=NO

# Use: -destination "id=<foo>" for a specific device
# list destinations with: xcodebuild -showdestinations -scheme test
