#include "audiosetupform.h"

AudioSetupForm::AudioSetupForm(AudioDriver* audioDrv) :
	table(6, 2, false), buttonOk(Gtk::Stock::OK), buttonApply(Gtk::Stock::APPLY),
	butBox(true, 10), butAlign(Gtk::ALIGN_RIGHT, Gtk::ALIGN_CENTER, 0.0, 0.0)
{
	audioDriver = audioDrv;
	set_title("Ustawienia urzadzen dzwiekowych");
	set_position(Gtk::WIN_POS_CENTER);
	
	labelHost.set_text("Typ sterownika");
	labelHost.set_alignment(1, 0.5);
	labelInDevice.set_text("Urzadzenie wejsciowe");
	labelInDevice.set_alignment(1, 0.5);
	labelOutDevice.set_text("Urzadzenie wyjsciowe");
	labelOutDevice.set_alignment(1, 0.5);
	labelSR.set_text("Czestotliwosc probkowania");
 	labelSR.set_alignment(1, 0.5);
 	labelLatencies.set_alignment(0, 0.5);
	
	// przyciski
	buttonApply.set_sensitive(false);
	butBox.pack_start(buttonApply, Gtk::PACK_EXPAND_WIDGET);
	butBox.pack_start(buttonOk, Gtk::PACK_EXPAND_WIDGET);
	butAlign.add(butBox);

	cboxSR.append_text("-13131");
	cboxSR.append_text("22050");
	cboxSR.append_text("44100");
	cboxSR.append_text("48000");
	cboxSR.set_active_text("44100");

	table.attach(labelHost, 0, 1, 0, 1, Gtk::FILL, Gtk::SHRINK, 0, 0);
	table.attach(cboxHost, 1, 2, 0, 1, Gtk::FILL, Gtk::SHRINK, 0, 0);
	table.attach(labelInDevice, 0, 1, 1, 2, Gtk::FILL, Gtk::SHRINK, 0, 0);
	table.attach(cboxInDevice, 1, 2, 1, 2, Gtk::FILL, Gtk::SHRINK, 0, 0);
	table.attach(labelOutDevice, 0, 1, 2, 3, Gtk::FILL, Gtk::SHRINK, 0, 0);
	table.attach(cboxOutDevice, 1, 2, 2, 3, Gtk::FILL, Gtk::SHRINK, 0, 0);
	table.attach(labelSR, 0, 1, 3, 4, Gtk::FILL, Gtk::SHRINK, 0, 0);
	table.attach(cboxSR, 1, 2, 3, 4, Gtk::FILL, Gtk::SHRINK, 0, 0);
	table.attach(labelLatencies, 0, 2, 4, 5, Gtk::FILL, Gtk::SHRINK, 0, 0);
	table.attach(butAlign, 0, 2, 5, 6, Gtk::FILL, Gtk::SHRINK, 0, 0);

	table.set_row_spacings(10);
	table.set_col_spacings(5);
	table.set_border_width(5);

	UpdateLatencies();

	add(table);

	// zapelnianie ComboBoxow nazwami sterownikow i urzadzen
	for(int i = 0; i < audioDriver->GetHostCount(); i++) {
		AddHostName( audioDriver->GetHostName(i) );
	}
	for(int i = 0; i < audioDriver->GetInputDeviceCount(); i++) {
		AddInputDeviceName( audioDriver->GetInputDeviceName(i) );
	}
	for(int i = 0; i < audioDriver->GetOutputDeviceCount(); i++) {
		AddOutputDeviceName( audioDriver->GetOutputDeviceName(i) );
	}
	
	// wybor na listach bierzacego sterownika i urzadzen audio
	cboxHost.set_active( audioDriver->GetCurrentHostNum() );
	cboxInDevice.set_active( audioDriver->GetCurrentInputDeviceNum() );
	cboxOutDevice.set_active( audioDriver->GetCurrentOutputDeviceNum() );
	// czest. probkowania hmmm... brzydko
	char txt[10]; g_snprintf( txt, 10, "%#d", (int)audioDriver->GetSampleRate() );
	cboxSR.set_active_text(txt);

	// sygnaly przyciskow i comboboxow
	buttonApply.signal_clicked().connect( sigc::mem_fun(this, &AudioSetupForm::OnApply) );
	buttonOk.signal_clicked().connect( sigc::mem_fun(this, &AudioSetupForm::OnOk) );
	cboxHost.signal_changed().connect( sigc::mem_fun(this, &AudioSetupForm::OnHostChanged) );
	cboxInDevice.signal_changed().connect( sigc::mem_fun(this, &AudioSetupForm::OnChanged) );
	cboxOutDevice.signal_changed().connect( sigc::mem_fun(this, &AudioSetupForm::OnChanged) );
	cboxSR.signal_changed().connect( sigc::mem_fun(this, &AudioSetupForm::OnChanged) );
		
	show_all_children();
}

AudioSetupForm::~AudioSetupForm() {
}

void AudioSetupForm::AddHostName(string name) {
    Glib::ustring str;
    str = Glib::locale_to_utf8(name);
	cboxHost.append_text(str);
}

void AudioSetupForm::AddInputDeviceName(string name) {
    Glib::ustring str;
    str = Glib::locale_to_utf8(name);
	cboxInDevice.append_text(str);
};

void AudioSetupForm::AddOutputDeviceName(string name) {
    Glib::ustring str;
    str = Glib::locale_to_utf8(name);
	cboxOutDevice.append_text(str);
};

void AudioSetupForm::OnApply() {
	int inDevNum, outDevNum;
	double sRate;

	sRate = atof( cboxSR.get_active_text().c_str() );
	inDevNum = cboxInDevice.get_active_row_number();
	outDevNum = cboxOutDevice.get_active_row_number();

	audioDriver->SetDevices(inDevNum, outDevNum);
	audioDriver->SetSampleRate(sRate);
	
	if( !audioDriver->IsConfigValid() ) {
		buttonOk.set_sensitive(false);
		// Gtk::Stock::DIALOG_ERROR
		Gtk::MessageDialog dialog(*this, "Portaudio", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
		dialog.set_secondary_text( audioDriver->GetLastErrorDescription() );
		dialog.run();
	} else {
		audioDriver->Open();
		UpdateLatencies();
        buttonOk.set_sensitive(true);
	}

	buttonApply.set_sensitive(false);
	audioDriver->PrintConfiguration();
}

void AudioSetupForm::OnHostChanged() {
	audioDriver->SetHost( cboxHost.get_active_row_number() );
	
	cboxInDevice.clear();
	cboxOutDevice.clear();
	
	for(int i = 0; i < audioDriver->GetInputDeviceCount(); i++) {
		AddInputDeviceName( audioDriver->GetInputDeviceName(i) );
	}
	for(int i = 0; i < audioDriver->GetOutputDeviceCount(); i++) {
		AddOutputDeviceName( audioDriver->GetOutputDeviceName(i) );
	}
	
	cboxInDevice.set_active( 0 );
	cboxOutDevice.set_active( 0 );
}

void AudioSetupForm::OnChanged() {
	buttonApply.set_sensitive(true);
}

void AudioSetupForm::OnOk() {
   	hide();
}

void AudioSetupForm::UpdateLatencies() {
	audioDriver->Open(); // konieczne by pobrac opoznienia
    char txt[120];
    g_snprintf(txt, 120, "Opoznienie wejsciowe: %#.2f ms\nOpoznienie wyjsciowe: %#.2f ms",
		audioDriver->GetInputLatency()*1000, audioDriver->GetOutputLatency()*1000);
	labelLatencies.set_text(txt);
}

