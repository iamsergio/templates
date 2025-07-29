#pragma once

#include <QCoreApplication>
#include <QDebug>
#include <QOpenGLContext>
#include <QStandardPaths>
#include <qopenglfunctions.h>

void printShaderCacheInfos(QOpenGLContext *ctx) {
  assert(ctx);

  GLint fmtCount = 0;
  ctx->functions()->glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &fmtCount);

  qDebug() << "GL_OES_get_program_binary="
           << ctx->hasExtension("GL_OES_get_program_binary")
           << "GL_ARB_get_program_binary="
           << ctx->hasExtension("GL_ARB_get_program_binary")
           << "; fmtCount=" << fmtCount << "; isGLES=" << ctx->isOpenGLES()
           << "; if gles, version3 has support="
           << QStringLiteral("%1.%2")
                  .arg(ctx->format().majorVersion())
                  .arg(ctx->format().minorVersion())
           << "QT_DISABLE_SHADER_DISK_CACHE="
           << qEnvironmentVariableIntValue("QT_DISABLE_SHADER_DISK_CACHE")
           << "Qt::AA_DisableShaderDiskCache="
           << QCoreApplication::testAttribute(Qt::AA_DisableShaderDiskCache)
           << "; GenericCacheLocation="
           << QStandardPaths::writableLocation(
                  QStandardPaths::GenericCacheLocation);
}
