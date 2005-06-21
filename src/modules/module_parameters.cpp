#include "module_parameters.h"

Parameter::Parameter(string type_, string name_) {
	type = type_;
	name = name_;
	description =  "";
	label = "";
}

Parameter::~Parameter() {
}

int Parameter::GetID() const {
	return id;
}

string Parameter::GetName() const {
	return name;
}

string Parameter::GetType() const {
	return type;
}
string Parameter::GetLabel() const {
	return label;
}

string Parameter::GetDescription() const {
	return description;
}

void Parameter::SetID(int newID) {
	id = newID;
}

void Parameter::SetLabel(string newLabel) {
	label = newLabel;
}

void Parameter::SetDescription(string newDesc) {
	description = newDesc;
}

//------------------------------------------------------------------------------
ParameterFloat::ParameterFloat(string name_) :
	Parameter("float", name_)
{
    value = 0;
    bounded = false;
}

void ParameterFloat::Bound(float min, float max, float stp) {
	minValue = min;
	maxValue = max;
	step = stp;
	bounded = true;
}

float ParameterFloat::GetMin() const {
	return minValue;
}

float ParameterFloat::GetMax() const {
	return maxValue;
}

float ParameterFloat::GetStep() const {
	return step;
}

//------------------------------------------------------------------------------
ParameterString::ParameterString(string name_) :
	Parameter("string", name_)
{
	description = "no description";
	label       = "no label";
	text        = "";
}

ParameterString::~ParameterString() {
}

string ParameterString::GetText() {
	return text;
}

void ParameterString::SetText(string newText) {
     text = newText;
}


