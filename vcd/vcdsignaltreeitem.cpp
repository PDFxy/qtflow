#include <QStringList>

#include "vcd/vcd_data.hpp"

#include "vcdsignaltreeitem.h"

VcdSignalTreeItem::VcdSignalTreeItem(const QList<QVariant> &data, VcdSignalTreeItem *parent)
{
	m_parentItem = parent;
	m_itemData = data;
}

VcdSignalTreeItem::~VcdSignalTreeItem()
{
	qDeleteAll(m_childItems);
}

void VcdSignalTreeItem::appendChild(vcd::Var var)
{
	QList<QVariant> data;
	data << QString::fromStdString(var.str_type());
	data << QString::fromStdString(var.name());
	m_childItems.append(new VcdSignalTreeItem(data,this));
}

VcdSignalTreeItem *VcdSignalTreeItem::child(int row)
{
	return m_childItems.value(row);
}

int VcdSignalTreeItem::childCount() const
{
	return m_childItems.count();
}

int VcdSignalTreeItem::columnCount() const
{
	return m_itemData.count();
}

QVariant VcdSignalTreeItem::data(int column) const
{
	return m_itemData.value(column);
}

VcdSignalTreeItem *VcdSignalTreeItem::parentItem()
{
	return m_parentItem;
}

int VcdSignalTreeItem::row() const
{
	if (m_parentItem)
		return m_parentItem->m_childItems.indexOf(const_cast<VcdSignalTreeItem*>(this));

	return 0;
}
