#ifndef I8080_WIDGET_MANAGER_H
#define I8080_WIDGET_MANAGER_H

#include "IWidget.h"
#include "Utils/Text/Utils.Text.h"
#include "Utils/Timer/Utils.Timer.Framerate.h"
#include <vector>
#include <string>


class WidgetManager : public SaveSystem {
public:
	WidgetManager(FPS_Timer* fps_limiter);
	~WidgetManager();

	void Draw();
	void Update();

	void AddWidgetPtr(IWidget* ptr_widget);
	IWidget* GetWidgetPtrByName(const std::string& Name);


	void LoadVisibilitySettings(std::vector<std::pair<std::string, bool>>);
	std::vector<std::pair<std::string, bool>> GetVisibilitySettings();


	bool GetFlag_AllOpened();

	void CloseAll();
	void OpenAll();

	int GetIndexSaveObject(const std::string& NameObject);


	nlohmann::json Save() override;
	void Load(const nlohmann::json& Data) override;

private:
	FPS_Timer* fps_limiter;


	std::vector<IWidget*> ptr_widgets;

	int indexHelp = -1;


	int FindIndex(std::string nameWidget);
};

#endif // !I8080_WIDGET_MANAGER_H
