cd build-rel && \
pkgbuild --root test.app \
     --identifier com.iamserj.test \
     --version 1.0.0 \
     --install-location /Applications/MyTest.app \
     test-unsigned.pkg && \
productsign --sign $APPLE_DEV_ID_INSTALLER test-unsigned.pkg test.pkg && \
xcrun notarytool submit  test.pkg --apple-id $APPLE_ID --team-id $APPLE_TEAM_ID --password $APPLE_SPECIFIC_PASSWORD --wait && \
xcrun stapler staple -v test.pkg
