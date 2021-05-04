#pragma once
#include "wx/wx.h"
#include <wx/editlbox.h>
#include <thread>
#include <future>
#include <map>

class cMain : public wxFrame
{
public:
	cMain();
	~cMain();
	wxButton *m_btn_server_update = nullptr;
	wxButton *m_btn_edit_server = nullptr;
	wxButton *m_btn_add_server = nullptr;
	wxButton *m_btn_start_server = nullptr;
	wxButton *m_btn_stop_server = nullptr;
	wxButton *m_btn_workshop_download = nullptr;
	wxTextCtrl *m_txt1 = nullptr;
	wxListBox *m_list1 = nullptr;

	void OnButtonClicked(wxCommandEvent &evt);


	wxDECLARE_EVENT_TABLE();
};