#ifndef NOTIFICATION_MANAGER_H
#define NOTIFICATION_MANAGER_H

#include "ThirdParty/ImGui/imgui.h"
#include "IDE/Core/Setting/interfaces/ISettingObject.h"
#include "IDE/Core/SaveSystem/SaveSystem.h"
#include "Utils/OpenGL/Window/Window.h"
#include "Utils/Timer/Utils.Timer.Framerate.h"
#include "Notification.h"
#include <iostream>	
#include <vector>


enum class NotificationType {
	Default,
	Error,
	Success
};


class NotificationManager : public ISettingObject
{
public:
	NotificationManager(Window* MainWindow, FPS_Timer* fps_limiter);
	~NotificationManager();


	void AddNottification(const Notification& new_Notification);

	void Draw();
	void Update();

	void SetFlag_Enabled(const bool& flag);
	void ToggleFlag_Enabled();
	bool GetFlag_Enable();



	void DrawSetting() override;

	nlohmann::json SaveSetting() override;
	void LoadSetting(const nlohmann::json& Data) override;

private:
	std::vector<Notification> notifs;

	bool isEnabled = true;

	float distance_between = 20.f;

	const int maxCount = 4;

	Window* MainWindow;
	FPS_Timer* fps_limiter;
};





#endif // !NOTIFICATION_MANAGER_H
