// Copyright( C ) 2019 Christian Riggenbach
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

#ifndef SECTIONCONTROLMODEL_H
#define SECTIONCONTROLMODEL_H

#include <QObject>

#include "../gui/SectionControlToolbar.h"

#include "BlockBase.h"

class SectionControlModel : public BlockBase {
    Q_OBJECT

  public:
    explicit SectionControlModel( QWidget* parent, QVBoxLayout* vLayout )
      : BlockBase() {
      sectionControlToolbar = new SectionControlToolbar( parent );
//      sectionControlToolbar->setVisible( true );
      vLayout->addWidget( sectionControlToolbar );
    }

    ~SectionControlModel() {
      sectionControlToolbar->deleteLater();
    }


  public slots:
    void setName( QString name ) {
      if( name == QStringLiteral( "SectionControl Model" ) ) {
        sectionControlToolbar->setName( QString() );
      } else {
        sectionControlToolbar->setName( name );
      }
    }
    void setNumberOfSections( float number ) {
      sectionControlToolbar->setNumberOfSections( number );
    }

  signals:
    void sectionStateChanged( int, int );

  public:
    SectionControlToolbar* sectionControlToolbar = nullptr;
};

class SectionControlModelFactory : public BlockFactory {
    Q_OBJECT

  public:
    SectionControlModelFactory( QWidget* parent, QVBoxLayout* vlayout )
      : BlockFactory(),
        parent( parent ), vlayout( vlayout ) {}

    QString getNameOfFactory() override {
      return QStringLiteral( "SectionControl Model" );
    }

    virtual void addToCombobox( QComboBox* combobox ) override {
      combobox->addItem( getNameOfFactory(), QVariant::fromValue( this ) );
    }

    virtual BlockBase* createNewObject() override {
      return new SectionControlModel( parent, vlayout );
    }

    virtual QNEBlock* createBlock( QGraphicsScene* scene, QObject* obj ) override {
      QNEBlock* b = new QNEBlock( obj );
      scene->addItem( b );

      b->addPort( getNameOfFactory(), QStringLiteral( "" ), 0, QNEPort::NamePort );
      b->addPort( getNameOfFactory(), QStringLiteral( "" ), 0, QNEPort::TypePort );

      b->addInputPort( "Number Of Sections", SLOT( setNumberOfSections( float ) ) );

      return b;
    }

  private:
    QWidget* parent = nullptr;
    QVBoxLayout* vlayout = nullptr;
};

#endif // SECTIONCONTROLMODEL_H