/*
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "controlpanel.h"
#include "ui_controlpanel.h"
#include "scene.h"

ControlPanel::ControlPanel(Scene *scene, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ControlPanel)
    , m_scene(scene)
{
    ui->setupUi(this);

    connect(scene, &Scene::lightChanged, this, &ControlPanel::updateSliders);

    connect(ui->ambientLightSlider, &QSlider::valueChanged, [scene] (int value) {
        Light l = scene->light();
        float v = value / 100.0;
        l.ambient = {v, v, v};
        scene->setLight(l);
    });

    connect(ui->lightDiffuseSlider, &QSlider::valueChanged, [scene] (int value) {
        Light l = scene->light();
        float v = value / 100.0;
        l.diffuse = {v, v, v};
        scene->setLight(l);
    });

    connect(ui->lightSpecularSlider, &QSlider::valueChanged, [scene] (int value) {
        Light l = scene->light();
        float v = value / 100.0;
        l.specular = {v, v, v};
        scene->setLight(l);
    });

    connect(ui->lightPosXSlider, &QSlider::valueChanged, [scene] (int value) {
        Light l = scene->light();
        l.position.setX(value / 100.0);
        scene->setLight(l);
    });

    connect(ui->lightPosYSlider, &QSlider::valueChanged, [scene] (int value) {
        Light l = scene->light();
        l.position.setY(value / 100.0);
        scene->setLight(l);
    });

    connect(ui->lightPosZSlider, &QSlider::valueChanged, [scene] (int value) {
        Light l = scene->light();
        l.position.setZ(value / 100.0);
        scene->setLight(l);
    });

    updateSliders();
}

ControlPanel::~ControlPanel()
{
    delete ui;
}

void ControlPanel::updateSliders()
{
    Light l = m_scene->light();
    ui->ambientLightSlider->setValue(l.ambient.x() * 100);
    ui->lightSpecularSlider->setValue(l.specular.x() * 100);
    ui->lightDiffuseSlider->setValue(l.diffuse.x() * 100);
    const QVector3D lightPos = l.position;
    ui->lightPosXSlider->setValue(lightPos.x() * 100);
    ui->lightPosYSlider->setValue(lightPos.y() * 100);
    ui->lightPosZSlider->setValue(lightPos.z() * 100);
}
