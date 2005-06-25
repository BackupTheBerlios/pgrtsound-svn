#ifndef AUDIOSETUPFORM_H
#define AUDIOSETUPFORM_H

#include <string>
#include <vector>

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/frame.h>
#include <gtkmm/label.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/table.h>
#include <gtkmm/enums.h>
#include <gtkmm/button.h>
#include <gtkmm/stock.h>
#include <gtkmm/alignment.h>
#include <gtkmm/messagedialog.h>

#include <glibmm/ustring.h>
#include <glib.h>

#include <audiodriver.h>



typedef std::vector<string> StringVector;
typedef StringVector::iterator SVIterator;

class AudioSetupForm : public Gtk::Window
{
	public:
		AudioSetupForm(AudioDriver* audioDrv);
		~AudioSetupForm();
		void AddHostName(string name);
		void AddInputDeviceName(string name);
		void AddOutputDeviceName(string name);
		void UpdateLatencies();
		void OnApply();
		void OnOk();
		void OnHostChanged();
		void OnChanged();
		
	private:
		AudioDriver* audioDriver;
//		Gtk::HBox boxSR, boxInDevice, boxOutDevice;
		Gtk::VBox box;
		Gtk::Label labelHost, labelInDevice, labelOutDevice, labelSR;
		Gtk::ComboBoxText cboxHost, cboxInDevice, cboxOutDevice, cboxSR;
		Gtk::Table table;
		Gtk::Alignment butAlign;
		Gtk::HBox butBox;
		Gtk::Button buttonApply, buttonOk;
		Gtk::Label labelLatencies;
};

#endif // AUDIOSETUPFORM_H
