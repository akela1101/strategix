#ifndef _MAINFORM_H
#define _MAINFORM_H

#include <Strategix_Forward.h>
#include <MapInfo_Forward.h>
#include <ui_MainForm.h>


namespace map_editor
{
using namespace std;
using namespace map_info;

class EditorMapWidget;

class MainForm : public QMainWindow
{
Q_OBJECT
	
	Ui::MainForm widget;
	EditorMapWidget* mapWidget;
	
	bool isMapOpened, isMapSaved;
	QString mapPath;
	umap<const QListWidgetItem*, ToolInfo*> toolFromItem;
	umap<const QPushButton*, int> playerNumbers;
	u_p<strx::Map> map;

public:
	MainForm();
	~MainForm();

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
	void CurrentToolChanged(ToolInfo* tool);
	void CurrentPlayerChanged(int playerNumber);
};

}

#endif    /* _MAINFORM_H */
