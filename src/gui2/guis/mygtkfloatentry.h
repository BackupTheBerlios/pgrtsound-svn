#ifndef MYGTKFLOATENTRY_H
#define MYGTKFLOATENTRY_H

#include "../../modules/module_parameters.h"
#include <gtkmm/entry.h>

class MyGtkFloatEntry : public Gtk::Entry
{
	public:
		MyGtkFloatEntry() {};
		
		~MyGtkFloatEntry() {};

		void AttachFloatParameter( ParameterFloat* param ) {
			pValue = param;
			SetValue( pValue->GetValue() );
		}
		
		void SetValue( float value ) {
   			char txt[50];
		    g_snprintf( txt, 50, "%#.2f", value );
			set_text( txt );
			pValue->SetValue( value );
		}

		float GetValue() {
			Glib::ustring str = get_text();
			float value = atof( str.c_str() );
			SetValue( value );
			return value;
		}
		
		void Update() {
			GetValue();
		}

//		bool on_key_press_event( GdkEventKey* event ) {
//			if( event->keyval ==  GDK_Return ) {
//				float x = GetValue();
//				//SetValue( x );
//				//cout << "x = " << x << endl;
//				pValue->SetValue( x );
//			}
//
//			return Gtk::Entry::on_key_press_event( event );
//		}

	protected:
		ParameterFloat* pValue;
};

#endif // MYGTKFLOATENTRY_H
