#include <QtCore>
#include <QNetworkProxy>
#include <QNetworkProxyFactory>
#include <QDebug>


QString proxyTypeToString(QNetworkProxy::ProxyType t)
{
    switch (t) {
    case QNetworkProxy::DefaultProxy:
        return "default";
    case QNetworkProxy::Socks5Proxy:
        return "Socks5Proxy";
    case QNetworkProxy::NoProxy:
        return "NoProxy";
    case QNetworkProxy::HttpProxy:
        return "HttpProxy";
    case QNetworkProxy::HttpCachingProxy:
        return "HttpCachingProxy";
    case QNetworkProxy::FtpCachingProxy:
        return "FtpCachingProxy";
    }
}

void printSystemProxies()
{
    QNetworkProxy proxy;
    qDebug() << "QNetworkProxy().hostName=" << proxy.hostName() << "QNetworkProxy().type=" << proxyTypeToString(proxy.type());
    qDebug() << "; applicationProxy=" << QNetworkProxy::applicationProxy();

    const auto systemProxies = QNetworkProxyFactory::systemProxyForQuery();
    for (int i = 0; i < systemProxies.count(); ++i) {
        auto proxy = systemProxies[i];
        qDebug() << i << proxy << proxy.hostName() << proxyTypeToString(proxy.type());
    }
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    printSystemProxies();

    return app.exec();
}
