#include "spriteidswidget.h"
#include "unitsconvert.h"

#include <QGridLayout>
#include <QLabel>
#include <QStringListModel>

SpriteIdWidget::SpriteIdWidget(QList<Sprite*> *sprites)
{
    this->sprites = sprites;

    QLabel* viewLabel = new QLabel(tr("View:"));
    viewLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QStringList viewTypes;
    viewTypes << tr("All in Level")
              << tr("Triggering IDs")
              << tr("Target IDs")
              << tr("Rotation IDs")
              << tr("Group IDs")
              << tr("Movement IDs")
              << tr("Path IDs");

    viewComboBox = new QComboBox();
    viewComboBox->addItems(viewTypes);
    connect(viewComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setView(int)));

    QLabel* searchLabel = new QLabel(tr("Search:"));
    searchLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    searchEdit = new QLineEdit();
    connect(searchEdit, SIGNAL(textEdited(QString)), this, SLOT(search(QString)));

    QGridLayout* topLayout = new QGridLayout();
    topLayout->setSpacing(6);
    topLayout->addWidget(viewLabel, 0, 0);
    topLayout->addWidget(viewComboBox, 0, 1);
    topLayout->addWidget(searchLabel, 1, 0);
    topLayout->addWidget(searchEdit, 1, 1);

    spriteTree = new QTreeWidget();
    spriteTree->setColumnCount(1);
    spriteTree->setHeaderHidden(false);
    spriteTree->setSortingEnabled(true);
    spriteTree->setRootIsDecorated(false);
    headerLables << tr("Sprite") << tr("ID");
    spriteTree->setHeaderLabels(headerLables);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addItem(topLayout);
    layout->addWidget(spriteTree);
    setLayout(layout);

    updateList();

    connect(spriteTree, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(handleIndexChange(QTreeWidgetItem*)));
}

void SpriteIdWidget::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        int currentView = viewComboBox->currentIndex();

        spriteData.loadSpriteDefs();
        spriteData.loadSpriteViews();

        viewComboBox->setCurrentIndex(currentView);

        updateList();
    }

    QWidget::changeEvent(event);
}

void SpriteIdWidget::search(QString text)
{
    for (int i = 0; i < spriteTree->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *item = spriteTree->topLevelItem(i);

        if (item->text(0).contains(text, Qt::CaseInsensitive) || item->text(1).contains(text, Qt::CaseInsensitive))
            item->setHidden(false);
        else
            item->setHidden(true);
    }
}

void SpriteIdWidget::updateEditor()
{
    updateList();
}

void SpriteIdWidget::updateList()
{
    handleChanges = false;
    QModelIndex index;
    if (spriteTree->selectionModel()->selectedIndexes().size() != 0) index = spriteTree->selectionModel()->selectedIndexes().at(0);
    spriteTree->clear();

    switch (viewMode)
    {
    case 1: // Has Triggering ID
    {
        foreach (Sprite* sprite, *sprites)
        {
            for (int i = 0; i < spriteData.getSpriteDef(sprite->getid()).getFieldCount(); i++)
            {
                Field* field = spriteData.getSpriteDef(sprite->getid()).getFieldPtr(i);

                QString match = "triggering";

                if (field->title.toLower().contains(match) && field->title.toLower().contains("id") && sprite->getNybbleData(field->startPos, field->endPos) != 0)
                {
                    QTreeWidgetItem* spriteItem = new QTreeWidgetItem();
                    spriteItem->setText(0, QString("%1: %2").arg(sprite->getid()).arg(spriteData.getSpriteDef(sprite->getid()).getName()));
                    spriteItem->setText(1, QString(tr("Triggering ID: %1")).arg(sprite->getNybbleData(field->startPos, field->endPos)));
                    spriteItem->setData(0, Qt::UserRole, sprite->getid());
                    spriteItem->setData(1, Qt::UserRole, sprite->getx());
                    spriteItem->setData(2, Qt::UserRole, sprite->gety());
                    spriteItem->setData(3, Qt::UserRole, sprite->getNybbleData(0, 23));
                    spriteTree->addTopLevelItem(spriteItem);
                }
            }
        }
        spriteTree->setColumnCount(2);
        spriteTree->setHeaderHidden(false);
        for(int i = 0; i < 2; i++)
            spriteTree->resizeColumnToContents(i);
        break;
    }
    case 2: // Has Target ID
    {
        foreach (Sprite* sprite, *sprites)
        {
            for (int i = 0; i < spriteData.getSpriteDef(sprite->getid()).getFieldCount(); i++)
            {
                Field* field = spriteData.getSpriteDef(sprite->getid()).getFieldPtr(i);

                QString match = "target";

                if (field->title.toLower().contains(match) && field->title.toLower().contains("id") && sprite->getNybbleData(field->startPos, field->endPos) != 0)
                {
                    QTreeWidgetItem* spriteItem = new QTreeWidgetItem();
                    spriteItem->setText(0, QString("%1: %2").arg(sprite->getid()).arg(spriteData.getSpriteDef(sprite->getid()).getName()));
                    spriteItem->setText(1, QString(tr("Target ID: %1")).arg(sprite->getNybbleData(field->startPos, field->endPos)));
                    spriteItem->setData(0, Qt::UserRole, sprite->getid());
                    spriteItem->setData(1, Qt::UserRole, sprite->getx());
                    spriteItem->setData(2, Qt::UserRole, sprite->gety());
                    spriteItem->setData(3, Qt::UserRole, sprite->getNybbleData(0, 23));
                    spriteTree->addTopLevelItem(spriteItem);
                }
            }
        }
        spriteTree->setColumnCount(2);
        spriteTree->setHeaderHidden(false);
        for(int i = 0; i < 2; i++)
            spriteTree->resizeColumnToContents(i);
        break;
    }
    case 3: // Has Rotation ID
    {
        foreach (Sprite* sprite, *sprites)
        {
            for (int i = 0; i < spriteData.getSpriteDef(sprite->getid()).getFieldCount(); i++)
            {
                Field* field = spriteData.getSpriteDef(sprite->getid()).getFieldPtr(i);

                QString match = "rotation";

                if (field->title.toLower().contains(match) && field->title.toLower().contains("id") && sprite->getNybbleData(field->startPos, field->endPos) != 0)
                {
                    QTreeWidgetItem* spriteItem = new QTreeWidgetItem();
                    spriteItem->setText(0, QString("%1: %2").arg(sprite->getid()).arg(spriteData.getSpriteDef(sprite->getid()).getName()));
                    spriteItem->setText(1, QString(tr("Rotation ID: %1")).arg(sprite->getNybbleData(field->startPos, field->endPos)));
                    spriteItem->setData(0, Qt::UserRole, sprite->getid());
                    spriteItem->setData(1, Qt::UserRole, sprite->getx());
                    spriteItem->setData(2, Qt::UserRole, sprite->gety());
                    spriteItem->setData(3, Qt::UserRole, sprite->getNybbleData(0, 23));
                    spriteTree->addTopLevelItem(spriteItem);
                }
            }
        }
        spriteTree->setColumnCount(2);
        spriteTree->setHeaderHidden(false);
        for(int i = 0; i < 2; i++)
            spriteTree->resizeColumnToContents(i);
        break;
    }
    case 4: // Has Group ID
    {
        foreach (Sprite* sprite, *sprites)
        {
            for (int i = 0; i < spriteData.getSpriteDef(sprite->getid()).getFieldCount(); i++)
            {
                Field* field = spriteData.getSpriteDef(sprite->getid()).getFieldPtr(i);

                QString match = "group";

                if (field->title.toLower().contains(match) && field->title.toLower().contains("id") && sprite->getNybbleData(field->startPos, field->endPos) != 0)
                {
                    QTreeWidgetItem* spriteItem = new QTreeWidgetItem();
                    spriteItem->setText(0, QString("%1: %2").arg(sprite->getid()).arg(spriteData.getSpriteDef(sprite->getid()).getName()));
                    spriteItem->setText(1, QString(tr("Group ID: %1")).arg(sprite->getNybbleData(field->startPos, field->endPos)));
                    spriteItem->setData(0, Qt::UserRole, sprite->getid());
                    spriteItem->setData(1, Qt::UserRole, sprite->getx());
                    spriteItem->setData(2, Qt::UserRole, sprite->gety());
                    spriteItem->setData(3, Qt::UserRole, sprite->getNybbleData(0, 23));
                    spriteTree->addTopLevelItem(spriteItem);
                }
            }
        }
        spriteTree->setColumnCount(2);
        spriteTree->setHeaderHidden(false);
        for(int i = 0; i < 2; i++)
            spriteTree->resizeColumnToContents(i);
        break;
    }
    case 5: // Has Movement ID
    {
        foreach (Sprite* sprite, *sprites)
        {
            for (int i = 0; i < spriteData.getSpriteDef(sprite->getid()).getFieldCount(); i++)
            {
                Field* field = spriteData.getSpriteDef(sprite->getid()).getFieldPtr(i);

                QString match = "movement";

                if (field->title.toLower().contains(match) && field->title.toLower().contains("id") && sprite->getNybbleData(field->startPos, field->endPos) != 0)
                {
                    QTreeWidgetItem* spriteItem = new QTreeWidgetItem();
                    spriteItem->setText(0, QString("%1: %2").arg(sprite->getid()).arg(spriteData.getSpriteDef(sprite->getid()).getName()));
                    spriteItem->setText(1, QString(tr("Movement ID: %1")).arg(sprite->getNybbleData(field->startPos, field->endPos)));
                    spriteItem->setData(0, Qt::UserRole, sprite->getid());
                    spriteItem->setData(1, Qt::UserRole, sprite->getx());
                    spriteItem->setData(2, Qt::UserRole, sprite->gety());
                    spriteItem->setData(3, Qt::UserRole, sprite->getNybbleData(0, 23));
                    spriteTree->addTopLevelItem(spriteItem);
                }
            }
        }
        spriteTree->setColumnCount(2);
        spriteTree->setHeaderHidden(false);
        for(int i = 0; i < 2; i++)
            spriteTree->resizeColumnToContents(i);
        break;
    }
    case 6: // Has Path ID
    {
        foreach (Sprite* sprite, *sprites)
        {
            for (int i = 0; i < spriteData.getSpriteDef(sprite->getid()).getFieldCount(); i++)
            {
                Field* field = spriteData.getSpriteDef(sprite->getid()).getFieldPtr(i);

                QString match = "path";

                if (field->title.toLower().contains(match) && field->title.toLower().contains("id") && sprite->getNybbleData(field->startPos, field->endPos) != 0)
                {
                    QTreeWidgetItem* spriteItem = new QTreeWidgetItem();
                    spriteItem->setText(0, QString("%1: %2").arg(sprite->getid()).arg(spriteData.getSpriteDef(sprite->getid()).getName()));
                    spriteItem->setText(1, QString(tr("Path ID: %1")).arg(sprite->getNybbleData(field->startPos, field->endPos)));
                    spriteItem->setData(0, Qt::UserRole, sprite->getid());
                    spriteItem->setData(1, Qt::UserRole, sprite->getx());
                    spriteItem->setData(2, Qt::UserRole, sprite->gety());
                    spriteItem->setData(3, Qt::UserRole, sprite->getNybbleData(0, 23));
                    spriteTree->addTopLevelItem(spriteItem);
                }
            }
        }
        spriteTree->setColumnCount(2);
        spriteTree->setHeaderHidden(false);
        for(int i = 0; i < 2; i++)
            spriteTree->resizeColumnToContents(i);
        break;
    }
    default: // All in Level
    {
        foreach (Sprite* sprite, *sprites)
        {
            QTreeWidgetItem* spriteItem = new QTreeWidgetItem();
            spriteItem->setText(0, QString("%1: %2").arg(sprite->getid()).arg(spriteData.getSpriteDef(sprite->getid()).getName()));
            spriteItem->setData(0, Qt::UserRole, sprite->getid());
            spriteItem->setData(1, Qt::UserRole, sprite->getx());
            spriteItem->setData(2, Qt::UserRole, sprite->gety());
            spriteItem->setData(3, Qt::UserRole, sprite->getNybbleData(0, 23));
            spriteTree->addTopLevelItem(spriteItem);
        }
        spriteTree->setColumnCount(1);
        spriteTree->setHeaderHidden(false);
        break;
    }
    }
    spriteTree->setCurrentIndex(index);

    handleChanges = true;
}

void SpriteIdWidget::deselect()
{
    spriteTree->clearSelection();
    updateList();
}

void SpriteIdWidget::setView(int view)
{
    switch (view) {
    case 1: // Has Triggering ID
        viewMode = 1;
        break;
    case 2: // Has Target ID
        viewMode = 2;
        break;
    case 3: // Has Rotation ID
        viewMode = 3;
        break;
    case 4: // Has Group ID
        viewMode = 4;
        break;
    case 5: // Has Movement ID
        viewMode = 5;
        break;
    case 6: // Has Path ID
        viewMode = 6;
        break;
    default: // All in Level
        viewMode = 0;
        break;
    }
    deselect();
}

void SpriteIdWidget::handleIndexChange(QTreeWidgetItem *spriteItem)
{
    if (handleChanges)
    {
        foreach (Sprite* sprite, *sprites)
        {
            if ((spriteItem->data(0, Qt::UserRole) == sprite->getid())
             && (spriteItem->data(1, Qt::UserRole) == sprite->getx())
             && (spriteItem->data(2, Qt::UserRole) == sprite->gety())
             && (spriteItem->data(3, Qt::UserRole) == sprite->getNybbleData(0, 23)))
            {
                emit selectedSpriteChanged(sprite);
            }
        }
        emit updateLevelView();
    }
}
