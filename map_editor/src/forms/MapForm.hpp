#pragma once

#include <strx/strx_fwd.hpp>
#include <ui_MapForm.h>

#include "MapInfo_fwd.hpp"


namespace map_editor
{
using namespace std;
using namespace map_info;

class EditorMapWidget;

class MapForm : public QMainWindow
{
	Q_OBJECT

	Ui::MapForm widget;
	EditorMapWidget* mapWidget;

	bool isMapOpened, isMapSaved;
	QString mapPath;
	umap<const QListWidgetItem*, ToolInfo*> toolFromItem;
	umap<const QPushButton*, int> playerNumbers;
	s_p<strx::Map> map;

public:
	MapForm();
	~MapForm();

public slots:
	void MapChanged(bool yes = true);

private slots:
	void FileNew();
	void FileSave();
	void FileLoad();
	void FileExit();
	void HelpAbout();

	void CurrentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
	void CurrentToolboxItemChanged(int index);
	void PlayerButtonToggled(bool on);

private:
	void PlacePlayerMarks();
	void ListWidgetFillMark(const string& filePath, QListWidget* listWidget);
	void ListWidgetFill(ToolType type, const string& name, QListWidget* listWidget);
	QListWidgetItem* AddToListWidget(const string& name, const QPixmap& pixmap, QListWidget* listWidget);
	bool TrySaveMap();
	QString SaveMap();

signals:
	void CurrentToolChanged(map_info::ToolInfo* tool);
	void CurrentPlayerChanged(int playerNumber);
};

}  // namespace map_editor
