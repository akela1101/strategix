#ifndef _EditorMapWidget_H
#define _EditorMapWidget_H

#include <MapWidget.h>


namespace map_editor
{
using namespace map_info;

class EditorMapWidget : public MapWidget
{
	friend class EditorGameWidget;
	
	using MapWidget::MapWidget;
	
	ToolInfo* tool = nullptr;     // current tool
	int playerId = 0;             // current player
	
	bool isHighlight = false;     // highlight cell under cursor
	MapCoord lastCoord;           // last cursor position in map coords
	QRect lastRc;                 // last rect under cursor

public slots:
	void CurrentToolChanged(ToolInfo* tool);
	void CurrentPlayerChanged(int playerId);

protected:
	void paintEvent(QPaintEvent*) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	
private:
	void UpdateUnderMouse(QMouseEvent* event);
	MapObject* CreateObject(int x, int y);
};
}

#endif //_EditorMapWidget_H