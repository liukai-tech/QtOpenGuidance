// Copyright( C ) 2020 Christian Riggenbach
//
// This program is free software:
// you can redistribute it and / or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// ( at your option ) any later version.
//
// This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY;
// without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see < https : //www.gnu.org/licenses/>.

#ifndef ORIENTATIONDOCKBLOCK_H
#define ORIENTATIONDOCKBLOCK_H

#include <QObject>
#include <QDockWidget>
#include <QSizePolicy>
#include <QMenu>

#include "../gui/MainWindow.h"
#include "../gui/OrientationDock.h"

#include "BlockBase.h"

#include "../kinematic/PoseOptions.h"

class OrientationDockBlock : public BlockBase {
    Q_OBJECT

  public:
    explicit OrientationDockBlock( MainWindow* mainWindow )
      : BlockBase() {
      widget = new OrientationDock( mainWindow );
      dock = new QDockWidget( mainWindow );
      dock->setWidget( widget );
    }

    ~OrientationDockBlock() {
      widget->deleteLater();
      dock->deleteLater();
    }


  public slots:
    void setName( const QString& name ) override {
      dock->setWindowTitle( name );
      action->setText( QStringLiteral( "Orientation: " ) + name );
      widget->setName( name );
    }

    void setOrientation( QQuaternion orientation ) {
      widget->setOrientation( orientation );
    }

    void setPose( const Point_3&, const QQuaternion orientation, const PoseOption::Options ) {
      widget->setOrientation( orientation );
    }


  public:

    void toJSON( QJsonObject& json ) override {
      QJsonObject valuesObject;

      valuesObject[QStringLiteral( "Font" )] = QJsonValue::fromVariant( QVariant( widget->fontOfLabel() ) );
      valuesObject[QStringLiteral( "Precision" )] = widget->precision;
      valuesObject[QStringLiteral( "Scale" )] = widget->scale;
      valuesObject[QStringLiteral( "FieldWitdh" )] = widget->fieldWidth;

      json[QStringLiteral( "values" )] = valuesObject;
    }

    void fromJSON( QJsonObject& json ) override {
      if( json[QStringLiteral( "values" )].isObject() ) {
        QJsonObject valuesObject = json[QStringLiteral( "values" )].toObject();

        widget->setFontOfLabel( valuesObject[QStringLiteral( "Font" )].toVariant().value<QFont>() );
        widget->precision = valuesObject[QStringLiteral( "Precision" )].toInt();
        widget->scale = valuesObject[QStringLiteral( "Scale" )].toDouble();
        widget->fieldWidth = valuesObject[QStringLiteral( "FieldWitdh" )].toInt();
      }
    }

    QDockWidget* dock = nullptr;
    QAction* action = nullptr;
    OrientationDock* widget = nullptr;
};

class OrientationDockBlockFactory : public BlockFactory {
    Q_OBJECT

  public:
    OrientationDockBlockFactory( MainWindow* mainWindow,
                                 Qt::DockWidgetArea area,
                                 Qt::DockWidgetAreas allowedAreas,
                                 QDockWidget::DockWidgetFeatures features,
                                 QMenu* menu )
      : BlockFactory(),
        mainWindow( mainWindow ),
        area( area ),
        allowedAreas( allowedAreas ),
        features( features ),
        menu( menu ) {}

    QString getNameOfFactory() override {
      return QStringLiteral( "OrientationDockBlock" );
    }

    virtual void addToCombobox( QComboBox* combobox ) override {
      combobox->addItem( getNameOfFactory(), QVariant::fromValue( this ) );
    }

    virtual BlockBase* createNewObject() override {
      return new OrientationDockBlock( mainWindow );
    }

    virtual QNEBlock* createBlock( QGraphicsScene* scene, QObject* obj ) override {
      auto* b = createBaseBlock( scene, obj );

      OrientationDockBlock* object = qobject_cast<OrientationDockBlock*>( obj );

      object->dock->setWidget( object->widget );
      object->dock->setFeatures( features );
      object->dock->setAllowedAreas( allowedAreas );
      object->dock->setObjectName( getNameOfFactory() + QString::number( b->id ) );

      object->action = object->dock->toggleViewAction();
      menu->addAction( object->action );

      mainWindow->addDockWidget( area, object->dock );

      b->addInputPort( QStringLiteral( "Orientation" ), QLatin1String( SLOT( setOrientation( QQuaternion ) ) ) );
      b->addInputPort( QStringLiteral( "Pose" ), QLatin1String( SLOT( setPose( const Point_3&, const QQuaternion, const PoseOption::Options ) ) ) );

      return b;
    }

  private:
    MainWindow* mainWindow = nullptr;
    Qt::DockWidgetArea area;
    Qt::DockWidgetAreas allowedAreas;
    QDockWidget::DockWidgetFeatures features;
    QMenu* menu = nullptr;
};

#endif // ORIENTATIONDOCKBLOCK_H