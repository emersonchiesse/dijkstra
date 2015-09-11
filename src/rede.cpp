
/////////////////////////////////////////////////////////////////////////////
// Name:        minimal.cpp
// Purpose:     Minimal wxWidgets sample
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: minimal.cpp 43915 2006-12-11 09:33:34Z CE $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "Grafo.h"
#include "Board.h"
#include "BasicDialog.h"
#include "ConfigDialog.h"
#include "Config.h"

#include "wx/dialog.h"
#include <algorithm>
// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------

// the application icon (under Windows and OS/2 it is in resources and even
// though we could still include the XPM here it would be unused)
//#if !defined(__WXMSW__) && !defined(__WXPM__)
//    #include "../sample.xpm"
//#endif

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:
    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit();
};



// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
public:
    // ctor(s)
    MyFrame(const wxString& title);

    // event handlers (these functions should _not_ be virtual)
    void OnOpen(wxCommandEvent& event);
    void OnDijkstra(wxCommandEvent& event);
    void OnRestart(wxCommandEvent& event);
    void OnRandom(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnConfig(wxCommandEvent& event);
    void OnDelete(wxCommandEvent& event);
    void OnShowRotas(wxCommandEvent& event);


    void OnPaint(wxPaintEvent& event);
    void OnMouse(wxMouseEvent& event);

    void OnKeyDown(wxKeyEvent& event);

private:
    // any class wishing to process wxWidgets events must use this macro
    DECLARE_EVENT_TABLE()
	Grafo grafo;
    std::vector<string> nodosSelecionados;
    setup::Config config;
};

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    Minimal_FileOpen = wxID_OPEN,

	Minimal_FileDikstra = wxID_FILE1,
	Minimal_restart = wxID_FILE2,
	Minimal_random = wxID_FILE3,
	Minimal_config = wxID_FILE4,
	Minimal_rotas,

	Minimal_del = wxID_DELETE,

    Minimal_Quit = wxID_EXIT,

    // it is important for the id corresponding to the "About" command to have
    // this standard value as otherwise it won't be handled properly under Mac
    // (where it is special and put into the "Apple" menu)
    Minimal_About = wxID_ABOUT
};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWidgets
// ----------------------------------------------------------------------------
#define MULTIPLIER 30

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
void MyFrame::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(this);

//    cout << "app.onpaint\n";

    wxSize size = GetClientSize();
    std::vector<Nodo> *nodos = &grafo.getNodos();

    // desenha nodos
    vector<Nodo>::iterator i;
    for (i=nodos->begin(); i< nodos->end(); i++)
    {
    	int x = (*i).getX();
    	int y = (*i).getY();
    	string id = (*i).getId();

    	// escolhe cor, se estiver selecionado
    	if (std::find(nodosSelecionados.begin(),
    					nodosSelecionados.end(),
						id) != nodosSelecionados.end() )
		{
			dc.SetBrush(*wxGREEN_BRUSH); // green filling
			dc.SetPen( wxPen( wxColor(255,0,0), 1 ) ); // 5-pixels-thick red outline
		}
    	else
    	{
			dc.SetBrush(*wxBLACK_BRUSH);
			dc.SetPen( wxPen( wxColor(255,0,0), 1 ) );
		}

    	// desenha nodos
    	dc.DrawCircle(x*MULTIPLIER, size.y-y*MULTIPLIER, config.getRaioNodo());
    	dc.DrawText(wxString::FromUTF8(id.c_str()),
    			x*MULTIPLIER+5, size.y-y*MULTIPLIER+5);

    	// desenha vertices
//    	std::vector<Vertice> vizinhos = nodos[i]->getVizinhos();
//    	for (int j=0; j<vizinhos.size(); j++)
    	vector<Vertice> *vizinhos = &(*i).getVizinhos();
		vector<Vertice>::const_iterator v;
		for (v = vizinhos->begin(); v != vizinhos->end(); v++)
    	{
    		//cout << "vizinho " << v << "\n";

    		string s=(*v). getId();
    		int peso=(*v).getCusto();
    		int ind = grafo.procura(s);
    		Nodo n = nodos->data()[ind];
    		int x1 = n.getX();
    		int y1 = n.getY();
    		dc.DrawLine(x*MULTIPLIER, size.y-y*MULTIPLIER,
    				x1*MULTIPLIER, size.y-y1*MULTIPLIER);

    		// escreve peso
    		if (config.isMostraPesos())
    		{
				dc.DrawText(wxString::Format(_T("%d"), peso),
						((x+x1)/2)*MULTIPLIER,
						size.y-((y+y1)/2)*MULTIPLIER);
    		}
    	}
    }
}

inline void MyFrame::OnKeyDown(wxKeyEvent& event) {

	switch (event.GetKeyCode())
	{
	case 127:
		wxMessageBox(wxString::Format("KeyDown: %i\n", (int)event.GetKeyCode()));
		break;

	default:
		break;
	}
	event.Skip();
}

inline void MyFrame::OnDelete(wxCommandEvent& event) {

	vector <string>::const_iterator i ;

	for (i = nodosSelecionados.begin (); i != nodosSelecionados.end();)
	{
//		if (grafo.delNodo(*i))
//			nodosSelecionados.erase(i);
	}

}

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_MENU(Minimal_FileOpen,  MyFrame::OnOpen)
	EVT_MENU(Minimal_FileDikstra,  MyFrame::OnDijkstra)
	EVT_MENU(Minimal_restart,  MyFrame::OnRestart)
	EVT_MENU(Minimal_random,  MyFrame::OnRandom)
    EVT_MENU(Minimal_Quit,  MyFrame::OnQuit)
    EVT_MENU(Minimal_About, MyFrame::OnAbout)
	EVT_MENU(Minimal_config, MyFrame::OnConfig)
	EVT_MENU(Minimal_del, MyFrame::OnDelete)
	EVT_MENU(Minimal_rotas, MyFrame::OnShowRotas)

    EVT_MOUSE_EVENTS(MyFrame::OnMouse)
END_EVENT_TABLE()

// Create a new application object: this macro will allow wxWidgets to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(MyApp)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    // call the base class initialization method, currently it only parses a
    // few common command-line options but it could be do more in the future
    if ( !wxApp::OnInit() )
        return false;

    // create the main application window
    MyFrame *frame = new MyFrame(_T("Redes Sem Fio App"));

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->SetSize(600, 600);
    frame->Show(true);

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned false here, the
    // application would exit immediately.
    return true;
}

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MyFrame::MyFrame(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title)//, setup::config()
{
	Connect(wxEVT_PAINT, wxPaintEventHandler(MyFrame::OnPaint));
//	wxPanel* mainPane = new wxPanel(this, wxID_ANY,
//			//wxDefaultPosition, wxDefaultSize,
//			10,10, wxWANTS_CHARS);
//	   mainPane->Bind(wxEVT_CHAR_HOOK, &MyFrame::OnKeyDown, this);

	//SetIcon(wxICON(sample));

#if wxUSE_MENUS
    // create a menu bar
    wxMenu *fileMenu = new wxMenu;
    wxMenu *toolsMenu = new wxMenu;
    wxMenu *dijMenu = new wxMenu;
    wxMenu *helpMenu = new wxMenu;

    helpMenu->Append(Minimal_About, _T("&About...\tF1"), _T("Show about dialog"));

    fileMenu->Append(Minimal_FileOpen, _T("&Open...\tF1"), _T("Open file"));
    fileMenu->Append(Minimal_Quit, _T("E&xit\tAlt-X"), _T("Quit this program"));

    toolsMenu->Append(Minimal_restart, _T("&Restart\tAlt-R"), _T(""));
    toolsMenu->Append(Minimal_random, _T("R&andom\tAlt-A"), _T(""));
    toolsMenu->Append(Minimal_FileDikstra, _T("&Dijkstra...\tF2"), _T("Calc Dijkstra"));
    toolsMenu->Append(Minimal_config, _T("&Configurações...\tF3"), _T("Configurações"));
    toolsMenu->Append(Minimal_del, _T("&Apaga nodo...\t"), _T("apaga nodo"));

    dijMenu->Append(Minimal_FileDikstra, _T("&Dijkstra...\tF2"), _T("Calc Dijkstra"));
    dijMenu->Append(Minimal_rotas, _T("&Rotas...\tF4"), _T("Mostra rotas"));

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, _T("&File"));
    menuBar->Append(toolsMenu, _T("&Ferramentas"));
    menuBar->Append(dijMenu , _T("&Dijkstra"));
    menuBar->Append(helpMenu, _T("&Help"));

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar(2);
    //SetStatusText(_T("Welcome to wxWidgets!"));
#endif // wxUSE_STATUSBAR

    //Board *board = new Board(this);
    //board->SetFocus();
 }


// event handlers
void MyFrame::OnOpen(wxCommandEvent& WXUNUSED(event))
{
//	wxTextCtrl *tc;
//	tc = new wxTextCtrl(this, -1, wxT(""), wxPoint(-1, -1),
//	    wxSize(-1, -1), wxTE_MULTILINE);

	wxFileDialog * openFileDialog = new wxFileDialog(this);
	if (openFileDialog->ShowModal() == wxID_OK){
		// apaga grafo ja carregado
		grafo.init();

	    wxString fileName = openFileDialog->GetPath();
	    grafo.load(string(fileName.ToAscii()));
	    grafo.loadcoords(string(fileName.ToAscii()));
	    this->Refresh(true);
	    this->Update();
	}
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxString::Format(
                    _T("Welcome to %s!\n")
                    _T("\n")
                    _T("\n")
                    _T("running under %s."),
                    wxVERSION_STRING,
                    wxGetOsDescription().c_str()
                 ),
                 _T("About Redes Sem Fio App"),
                 wxOK | wxICON_INFORMATION,
                 this);
}

void MyFrame::OnDijkstra(wxCommandEvent& event) {
	if (nodosSelecionados.size()==2)
	{
		cout << "\niniciando dijkstra:" << endl;

		grafo.caminhomaiscurto(nodosSelecionados[0],
				nodosSelecionados[1]);

		cout << "\ndijkstra done." << endl;

//		wxMessageBox(wxString::Format(
//		                    _T("Welcome to %s!\n")
//		                    _T("\n")
//		                    _T("\n")
//		                    _T("running under %s."),
//		                    wxVERSION_STRING,
//		                    wxGetOsDescription().c_str()
//		                 ),
//		                 _T("About Redes Sem Fio App"),
//		                 wxOK | wxICON_INFORMATION,
//		                 this);
	}
	else
		SetStatusText("selecione 2 nodos");
}


void MyFrame::OnRestart(wxCommandEvent& event) {

	// apaga todos os selecionados
	while (nodosSelecionados.size()>0)
		nodosSelecionados.pop_back();

	this->Refresh(true);
	this->Update();
}

void MyFrame::OnRandom(wxCommandEvent& event) {

    LinhasDialog aboutDialog ( this, -1, _("Digite numero de nós"),
    	                          wxPoint(100, 100), wxSize(200, 200) );
	if ( aboutDialog.ShowModal() != wxID_OK )
		SetStatusText(_("The about box was cancelled.\n"));
	else
	{
		string result = aboutDialog.GetText();
		int count = atoi(result.c_str());
		if (count <= 0)
			return;

		grafo.init();

		grafo.criaRandom(count, config.getRaioGrafo());

		this->Refresh(true);
		this->Update();
	}
}


void MyFrame::OnShowRotas(wxCommandEvent& event) {

	if (nodosSelecionados.size()==1)
	{
		LinhasDialog aboutDialog ( this, -1, _("Rotas"),
				wxDefaultPosition,
				//wxPoint(100, 100),
				wxSize(400, 400) );
		aboutDialog.setText(grafo.getRotas(nodosSelecionados[0]));
		if ( aboutDialog.ShowModal() != wxID_OK )
			SetStatusText(_("The about box was cancelled.\n"));
		else
		{
		}
	}
}



void MyFrame::OnConfig(wxCommandEvent& event) {
    ConfigDialog c ( this, -1, _("Digite numero de nós"),
    	                          wxPoint(100, 100), wxSize(300, 300) );
	if ( c.ShowModal() != wxID_OK )
		SetStatusText(_("The about box was cancelled.\n"));
	else
	{
		string result = c.GetText();

		this->Refresh(true);
		this->Update();
	}
}

void MyFrame::OnMouse(wxMouseEvent& event) {

	int ind = -1;
	{
		wxSize size = GetClientSize();
		int x = event.GetX();
		int y = event.GetY();
		SetStatusText(wxString::Format(_T("x: %d, y: %d"), x, y));

		// se esta proximo a nodo, escreve no status text
		ind = grafo.procura(x/MULTIPLIER, (size.y - y)/MULTIPLIER);
		if (ind >= 0)
		{
			std::vector<Nodo> nodos = grafo.getNodos();

			string id = nodos[ind].getId().c_str();
			SetStatusText(wxString::Format(_T("x: %d, y: %d, nodo: %s"),
					x, y, id));

			// se clicado perto do nodo, seleciona ou limpa seleção
			if (event.LeftUp())
			{
				// se estava selecionado, limpa
				// se nao estava selecionado, seleciona
				vector <string>::iterator i = nodosSelecionados.begin ();
				i = std::find(nodosSelecionados.begin(),
						nodosSelecionados.end(),
						id);
				if (i == nodosSelecionados.end() )
				{
					nodosSelecionados.push_back(id);
					cout << "\n selecionado nodo: " << id << endl;

					// vizinho do nodo:
					vector<Vertice> vertices = grafo.getVizinhos(id);
					//if (vertices != NULL)
					{
						cout << "vizinhos: " ;
						for (int w = 0; w < vertices.size(); w++)
							cout << vertices[w].getId() << " ";
						cout << endl;
					}
				}
				else
				{
					nodosSelecionados.erase(i);
					cout << "\n deselecionado nodo: " << id << endl;
				}


				this->Refresh(true);
				this->Update();
			}
		}
	}


}
