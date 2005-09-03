#ifndef GMCONSTANT_H
#define GMCONSTANT_H

#include "guimodule.h"
#include "mygtkfloatentry.h"

//class GConstant : public Gtk::Entry {
//	public:
//	    GConstant( Module* mod ) : module( mod ) {
//			pValue = (ParameterFloat*)module->GetParameter( 0 );
//			SetValue( pValue->GetValue() );
//		}
//		~GConstant() {};
//
//		void SetValue( float value ) {
//   			char txt[50];
//		    g_snprintf( txt, 50, "%#.2f", value );
//			set_text( txt );
//		}
//
//		float GetValue() {
//			Glib::ustring str = get_text();
//			float value = atof( str.c_str() );
//			return value;
//		}
//
//		bool on_key_press_event( GdkEventKey* event ) {
//			if( event->keyval ==  GDK_Return ) {
//				float x = GetValue();
//				cout << "Przetwarzaj: x = " << x << endl;
//				SetValue( x );
//				pValue->SetValue( x );
//			}
//
//			return Gtk::Entry::on_key_press_event( event );
//		}
//
//	protected:
//		Module* module;
//		My* pValue;
//};

/*
 * No description
 */
class GMConstant : public GuiModule
{
	public:
		GMConstant( Module* module );
		~GMConstant();
		Gtk::Widget* GetGui();
		
	protected:
		MyGtkFloatEntry entry;
};

#endif // GMCONSTANT_H
