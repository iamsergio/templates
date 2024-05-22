#include <QDebug>
#include <QtNetwork/QSslSocket>

int main()
{
    qDebug() << "supports:" << QSslSocket::supportsSsl() << "; runtime:" << QSslSocket::sslLibraryVersionString() << "; compiled:" << QSslSocket::sslLibraryBuildVersionString();

    return 0;
}
