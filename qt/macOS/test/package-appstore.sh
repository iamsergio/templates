script_dir=$(dirname "$(realpath "$0")")

cd $script_dir/build-rel && \
cp ../Entitlements.entitlements test.app/Contents/Resources/ && \
macdeployqt test.app -qmldir=../ -appstore-compliant -sign-for-notarization=$APPLE_MAC_APPLICATION && \
productbuild --sign $APPLE_MAC_INSTALLER --component test.app /Applications test-appstore.pkg && \
xcrun altool --upload-app -f test-appstore.pkg --type osx --username $APPLE_ID --password $APPLE_SPECIFIC_PASSWORD
