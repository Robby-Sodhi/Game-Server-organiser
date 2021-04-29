#include "cMain.hpp"


wxBEGIN_EVENT_TABLE(cMain, wxFrame)
	EVT_BUTTON(1,OnButtonClicked)
	EVT_BUTTON(2, OnButtonClicked)
	EVT_BUTTON(3, OnButtonClicked)
	EVT_BUTTON(4, OnButtonClicked)
wxEND_EVENT_TABLE()

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Test Application", wxPoint(50,50), wxSize(800,600))
{
	m_btn_server_update = new wxButton(this, 1, "Server Update", wxPoint(10, 10), wxSize(150, 50));
	m_btn_edit_server = new wxButton(this, 2, "Edit Server", wxPoint(10, 75), wxSize(150, 50));
	m_btn_add_server = new wxButton(this, 3, "Add Server", wxPoint(10, 140), wxSize(150, 50));
	m_btn_start_server = new wxButton(this, 4, "Start Server", wxPoint(10, 205), wxSize(150, 50));
	m_txt1 = new wxTextCtrl(this, wxID_ANY, "", wxPoint(300, 400), wxSize(300,30));
	m_list1 = new wxListBox(this, wxID_ANY, wxPoint(300, 10), wxSize(300,300));
}

cMain::~cMain()
{
}

void cMain::OnButtonClicked(wxCommandEvent & evt)
{
	switch (evt.GetId())
	{
	case 1:
		m_list1->AppendString("Server working");
		evt.Skip();
		break;
	case 2:
		m_list1->AppendString("Edit working");
		evt.Skip();
		break;
	case 3:
		m_list1->AppendString("Add working");
		evt.Skip();
		break;
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
}
