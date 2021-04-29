#pragma once
#include "wx/wx.h"
#include <wx/editlbox.h>
#include "unturnedServer.hpp"
#include "SteamCMD.hpp"

class cMain : public wxFrame
{
public:
	cMain();
	~cMain();
	wxButton *m_btn_server_update = nullptr;
	wxButton *m_btn_edit_server = nullptr;
	wxButton *m_btn_add_server = nullptr;
	wxButton *m_btn_start_server = nullptr;
	wxTextCtrl *m_txt1 = nullptr;
	wxListBox *m_list1 = nullptr;

	void OnButtonClicked(wxCommandEvent &evt);

	wxDECLARE_EVENT_TABLE();
};