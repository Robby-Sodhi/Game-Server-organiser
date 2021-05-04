#include "cMain.hpp"
#include "unturnedServer.hpp"
#include <iostream>

using namespace std;

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
	EVT_BUTTON(1,OnButtonClicked)
	EVT_BUTTON(2, OnButtonClicked)
	EVT_BUTTON(3, OnButtonClicked)
	EVT_BUTTON(4, OnButtonClicked)
	EVT_BUTTON(5, OnButtonClicked)
	EVT_BUTTON(6, OnButtonClicked)
wxEND_EVENT_TABLE()

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Test Application", wxPoint(50,50), wxSize(800,600))
{
	m_btn_server_update = new wxButton(this, 1, "Server Update", wxPoint(10, 10), wxSize(150, 50));
	m_btn_edit_server = new wxButton(this, 2, "Edit Server", wxPoint(10, 75), wxSize(150, 50));
	m_btn_add_server = new wxButton(this, 3, "Add Server", wxPoint(10, 140), wxSize(150, 50));
	m_btn_start_server = new wxButton(this, 4, "Start Server", wxPoint(10, 205), wxSize(150, 50));
	m_btn_stop_server = new wxButton(this, 5, "Stop Server", wxPoint(10, 270), wxSize(150, 50));
	m_btn_workshop_download = new wxButton(this, 6, "Start Server", wxPoint(10, 335), wxSize(150, 50));
	m_txt1 = new wxTextCtrl(this, wxID_ANY, "", wxPoint(300, 400), wxSize(300,30));
	m_list1 = new wxListBox(this, wxID_ANY, wxPoint(300, 10), wxSize(300,300));
}

cMain::~cMain()
{
}

void cMain::OnButtonClicked(wxCommandEvent & evt)
{
	bool threadFinished{ false };
	bool threadRunning{ false };
	std::thread update_app;
	std::vector<UnturnedServer> servers;

	if (threadFinished) {
		update_app.join();
		threadRunning = false;
		threadFinished = false;
	}

	//server update
	if (evt.GetId() == 1)
	{
		threadRunning = true;
		m_list1->AppendString("spinning thread");
		update_app = std::thread{ [&] {servers[0].update_app(); threadFinished = true; } };
		evt.Skip();
	}
	//edit server
	else if (evt.GetId() == 2)
	{
		servers[0].editProperties(std::map<std::string, std::string>{std::pair<std::string, std::string>{"map", "russia"}, std::pair<std::string, std::string>{"gold", "place"}});
	}
	//add server
	else if (evt.GetId() == 3)
	{
		std::string serverName{};
		std::string unturnedPath{};
		std::string steamCMDPath{};

		cin.ignore((numeric_limits<streamsize>::max)(), '\n');

		cout << "unturnedPath: ";
		std::getline(std::cin, unturnedPath);

		cout << "unturnedServerName: ";
		std::getline(std::cin, serverName);

		cout << "steamCMDPath: ";
		std::getline(std::cin, steamCMDPath);

		std::string serverPath = unturnedPath + "\\Servers\\" + serverName;


		UnturnedServer server = UnturnedServer(serverPath, unturnedPath, serverName, steamCMDPath);
		servers.push_back(server);
	}
	//start server
	else if (evt.GetId() == 4)
	{
		servers[0].startUnturnedServer();
	}
	//stop server
	else if (evt.GetId() == 5)
	{
		int result = servers[0].stopUnturnedServer();
		if (result == 0)
			m_list1->AppendString("failed");
		else if (result == 1)
			m_list1->AppendString("successful");
		else if (result == 2)
			m_list1->AppendString("fatal error can't recover");
	}
	//workshop download
	else if (evt.GetId() == 6)
	{
		cin.ignore((numeric_limits<streamsize>::max)(), '\n');

		std::cout << "workshop ID: ";
		std::string workshopLink;
		std::cin >> workshopLink;

		cin.ignore((numeric_limits<streamsize>::max)(), '\n');

		servers[0].workshopDownload(workshopLink);
	}
	/*
	switch (evt.GetId())
	{
	//serverUpdate
	case 1:
		m_list1->AppendString("Server working");
		evt.Skip();
		break;
	//editServer
	case 2:
		m_list1->AppendString("Edit working");
		evt.Skip();
		break;
	//addServer
	case 3:
		m_list1->AppendString("Add working");
		evt.Skip();
		break;
	//startServer
	case 4:
		m_list1->AppendString("Start working");
		evt.Skip();
		break;
	default:
		evt.Skip();
		break;
	}
	//m_list1->AppendString(m_txt1->GetValue());
	//evt.Skip();
	*/
}
