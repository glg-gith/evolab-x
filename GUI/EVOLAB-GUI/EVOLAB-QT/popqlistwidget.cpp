#include "popqlistwidget.h"

#include <LabConsts.h>

PopQListWidget::PopQListWidget(QWidget *parent) :
    QListWidget(parent)
{
}

//void PopQListWidget::updateForeignModelSignal(PointerLessModels::t_basic_item *p_item)
//{

//}

void PopQListWidget::EmitUpdateForeignModelSignal(int index, PointerLessModels::t_basic_item *p_item)
{
    cout << "Emitting updateForeignModelSignal(p_item) !! (on index: " << index << " | on item: " << p_item->name << ")" << endl;
    emit updateForeignModelSignal(index, p_item);
}

//void PopQListWidget::SetCurrentSelected(int index)
//{
//    this->currentSelected = index;
//}


void PopQListWidget::dropEvent(QDropEvent *event)
{
    //QListWidgetItem * item = (QListWidgetItem *)event->source()->currentItem();
    this->setItemSelected(this->currentItem(), true);
    int new_index = this->currentRow();
    //int new_index2 = item->;
    cout << "New Index: " << new_index << endl;
}
