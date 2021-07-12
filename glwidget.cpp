/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "glwidget.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMouseEvent>

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , program(0)
    , transX(0)
    , transY(0)
    , transZ(0)
    , rotX(0)
    , rotY(0)
    , rotZ(0)
    , m_pInterpreter(NULL){
}

GLWidget::~GLWidget()
{
    makeCurrent();
    delete program;
    doneCurrent();
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(200, 200);
}


//! [3]
static const char *vertexShaderSource =
    "attribute highp vec4 posAttr;\n"
    "attribute lowp vec4 colAttr;\n"
    "varying lowp vec4 col;\n"
    "uniform highp mat4 matrix;\n"
    "void main() {\n"
    "   col = colAttr;\n"
    "   gl_Position = matrix * posAttr;\n"
    "}\n";

static const char *fragmentShaderSource =
    "varying lowp vec4 col;\n"
    "void main() {\n"
    "   gl_FragColor = col;\n"
    "}\n";
//! [3]

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();
     program = new QOpenGLShaderProgram(this);
    program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    program->link();
    m_posAttr = program->attributeLocation("posAttr");
    m_colAttr = program->attributeLocation("colAttr");
    m_matrixUniform = program->uniformLocation("matrix");
}

void GLWidget::paintGL()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glLineWidth(2.0f);
    program->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, width()/height(), 0.1f, 1000.0f);
    matrix.translate(0, 0, -2);
    matrix.translate(transX/7.0f, transY/7.0, transZ/7.0f);
    matrix.rotate(rotX, 1, 0, 0);
    matrix.rotate(rotY, 0, 1, 0);
    matrix.rotate(rotZ, 0, 0, 1);

    float dx=(m_Min.x + m_Max.x)/2.0;
    float dy=(m_Min.y + m_Max.y)/2.0;
    float dz=(m_Min.z + m_Max.z)/2.0;
    matrix.translate(-dx, -dy, -dz);
    program->setUniformValue(m_matrixUniform, matrix);

    program->setUniformValue(m_matrixUniform, matrix);



    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    GLfloat axisVertices[] = {
         1.0f,   0.0f,  0.0f,
        -1.0f,   0.0f,  0.0f,
         0.0f,  -1.0f,  0,
         0.0f,   1.0f,  0,
         0.0f,   0.0f, -1.0f,
         0.0f,   0.0f,  1.0f
    };
    GLfloat axisColors[] = {
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
    };
    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, axisVertices);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, axisColors);

    glDrawArrays(GL_LINES, 0, 6);

    drawBoundingBox();

    drawModel();

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    program->release();
    update();
}


void GLWidget::drawModel(){
    //TODO directly use this variable
    vector<GCodeLine>& codeLines=m_GCodeLines;//interpreter.getCodeLines();
    if(m_GCodeLines.size() == 0){
        return;
    }
    float oldx=0, oldy=0, oldz=0;
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> colors;
    int maxLayer = 1000;
    int minLayer = 0;
    for(unsigned int i=0; i<codeLines.size(); i++)
    {
        if(codeLines[i].layer > layer){
            continue;
        }
        if(codeLines[i].command=="G1")  // draw a line
        {
            float x=oldx, y=oldy, z=oldz;
            bool hasE=false;
            for(unsigned int j=0; j<codeLines[i].parameters.size(); j++)
            {
                if(codeLines[i].parameters[j].name=='E')
                    hasE=true;
            }
            for(unsigned int j=0; j<codeLines[i].parameters.size(); j++)
            {
                if(codeLines[i].parameters[j].name=='X')
                    x=codeLines[i].parameters[j].value;
                if(codeLines[i].parameters[j].name=='Y')
                    y=codeLines[i].parameters[j].value;
                if(codeLines[i].parameters[j].name=='Z')
                    z=codeLines[i].parameters[j].value;
            }
            if(hasE)
            {
                vertices.push_back(oldx);
                vertices.push_back(oldy);
                vertices.push_back(oldz);
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);

                colors.push_back(1.0f);
                colors.push_back(0.0f);
                colors.push_back(0.0f);

                colors.push_back(1.0f);
                colors.push_back(0.0f);
                colors.push_back(0.0f);
            }
            oldx=x;
            oldy=y;
            oldz=z;
        }
    }
    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices.data());
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors.data());

    glDrawArrays(GL_LINES, 0, vertices.size()/3);
}

void GLWidget::drawBoundingBox(){
    GLfloat vertices[] = {
        m_Min.x,m_Min.y,m_Min.z,
        m_Min.x,m_Min.y,m_Max.z,

        m_Min.x,m_Min.y,m_Max.z,
        m_Max.x,m_Min.y,m_Max.z,

        m_Max.x,m_Min.y,m_Max.z,
        m_Max.x,m_Min.y,m_Min.z,

        m_Max.x,m_Min.y,m_Min.z,
        m_Min.x,m_Min.y,m_Min.z,

        // hinten
        m_Min.x,m_Max.y,m_Min.z,
        m_Min.x,m_Max.y,m_Max.z,

        m_Min.x,m_Max.y,m_Max.z,
        m_Max.x,m_Max.y,m_Max.z,

        m_Max.x,m_Max.y,m_Max.z,
        m_Max.x,m_Max.y,m_Min.z,

        m_Max.x,m_Max.y,m_Min.z,
        m_Min.x,m_Max.y,m_Min.z,

        // seiten
        m_Min.x,m_Min.y,m_Min.z,
        m_Min.x,m_Max.y,m_Min.z,

        m_Max.x,m_Min.y,m_Min.z,
        m_Max.x,m_Max.y,m_Min.z,

        m_Min.x,m_Min.y,m_Max.z,
        m_Min.x,m_Max.y,m_Max.z,

        m_Max.x,m_Min.y,m_Max.z,
        m_Max.x,m_Max.y,m_Max.z
    };
    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f
    };
    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    glDrawArrays(GL_LINES, 0, 24);

}

void GLWidget::onGCodeLoaded(){
    Q_ASSERT(m_pInterpreter != NULL);
    m_Min = m_pInterpreter->getMin();
    m_Max = m_pInterpreter->getMax();
    m_GCodeLines = m_pInterpreter->getCodeLines();
    updateLayerMinMax();

    emit layerMinMaxChanged(m_LayerMin, m_LayerMax);
}

void GLWidget::updateLayerMinMax() {
    m_LayerMin = 1000;
    m_LayerMax = -1000;
    for(int i=0; i<m_GCodeLines.size(); i++){
        if(m_GCodeLines[i].interprete == false){
            continue;
        }
        if(m_GCodeLines[i].layer < m_LayerMin){
            m_LayerMin = m_GCodeLines[i].layer;
        }
        if(m_GCodeLines[i].layer > m_LayerMax){
            m_LayerMax = m_GCodeLines[i].layer;
        }
    }
    layer = m_LayerMin;
}

void GLWidget::loadGCode(const QString &filename){
    if(m_pInterpreter != NULL){
        qDebug() << "Loading is under way";
        return;
    }
    m_pInterpreter = new GCodeInterpreter();
    connect(m_pInterpreter, &QThread::finished, this, &GLWidget::onGCodeLoaded);
    m_pInterpreter->setFileName(filename);

    m_pInterpreter->start();
}

void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
}


void GLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    //cout<<"Mouse released"<<endl;
    if(event->button()==Qt::LeftButton)
        leftMousePressed=false;
    else if(event->button()==Qt::MidButton)
        midMousePressed=false;
    else if(event->button()==Qt::RightButton)
        rightMousePressed=false;
}

void GLWidget::mousePressEvent(QMouseEvent* event) {
    //cout<<"Mouse pressed"<<endl;
    lastMousePosition=event->pos();
    if(event->button()==Qt::LeftButton)
        leftMousePressed=true;
    else if(event->button()==Qt::MidButton)
        midMousePressed=true;
    else if(event->button()==Qt::RightButton)
        rightMousePressed=true;
}

void GLWidget::mouseMoveEvent(QMouseEvent* event) {
    //cout<<transX<<"/"<<transY<<endl;
    if(midMousePressed)
    {
        transX+=-(lastMousePosition.x()-event->x());
        transY+=lastMousePosition.y()-event->y();
        update();
    }
    if(leftMousePressed)
    {
        rotX+=(lastMousePosition.y())-event->y();
        rotY+=(lastMousePosition.x())-event->x();
        //cout<<"rotY: "<<rotY<<endl;
        update();
    }
    lastMousePosition=event->pos();
}

void GLWidget::wheelEvent(QWheelEvent* event)
{
    transZ+=event->delta()/10.0;
    //cout<<"transZ: "<<transZ<<endl;
    update();
}

