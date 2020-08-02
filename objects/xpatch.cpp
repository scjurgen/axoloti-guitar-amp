//$MODULES=
//$MODULE_DIRS=
#include <new>
#include "/Users/scjurgen/Desktop/axoloti-guitar-amp/FilterChain.h"
#include "/Users/jurgenschwietering/Desktop/axoloti-guitar-amp/FilterChain.h"


#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#define MIDICHANNEL 0 // DEPRECATED!

static int32_t * AudioInputLeft;
static int32_t * AudioInputRight;
static int32_t * AudioOutputLeft;
static int32_t * AudioOutputRight;

static void ModulationSourceChange(PExModulationTarget_t *modulation,
                               int32_t nTargets,
                               Parameter_t *parameters,
                               int32_t *oldvalue,
                               int32_t value) {
  PExModulationTarget_t *s = modulation;
  int t;
  for (t = 0; t < nTargets; t++) {
    PExModulationTarget_t *target = &s[t];
    if (target->parameterIndex == -1)
      continue;
    Parameter_t *PEx = &parameters[target->parameterIndex];
    int32_t v = PEx->d.frac.modvalue;
    v -= ___SMMUL(*oldvalue, target->amount) << 5;
    v += ___SMMUL(value, target->amount) << 5;
    PEx->d.frac.modvalue = v;
    if (PEx->pfunction) {
      (PEx->pfunction)(PEx);
      // TBC: modulation on root of polyphonic-subpatch-parameters
    }
    else {
      PEx->d.frac.finalvalue = v;
    }
  }
  *oldvalue = value;
}

     typedef enum { A_STEREO, A_MONO, A_BALANCED } AudioModeType;
     static AudioModeType AudioOutputMode = A_STEREO;
     static AudioModeType AudioInputMode = A_STEREO;
static void PropagateToSub(Parameter_t *origin) {
      Parameter_t *p = (Parameter_t *)origin->d.frac.finalvalue;
      parameter_setVal(p,origin->d.frac.modvalue,0xFFFFFFEE);
}
class rootc : public PatchInstance {
public:
  void tick( int32_t * inbuf, int32_t * outbuf) {
    int32_t *p = outbuf;
    int i;
    for(i=0;i<BUFSIZE;i++){*p++=0;*p++=0;}
    AudioInputLeft = &inbuf[0];
    AudioInputRight = &inbuf[BUFSIZE];
    AudioOutputLeft = &outbuf[0];
    AudioOutputRight = &outbuf[BUFSIZE];
    dsp();
  }

  void midiInHandler(int32_t m) {
    midi_message_t m1;
    m1.word = m;
    midiInHandler(0, m1);
  }

  void* getProperty(ax_property_id_t id, int index) {
    switch(id) {
    case ax_prop_displayvector :
      return displayVector;
    case ax_prop_displayvector_size:
      return (void*)(sizeof(displayVector)/sizeof(int32_t));
    case ax_prop_nparams:
      return (void*)nparams;
    case ax_prop_param:
      return &PExch[index];
    case ax_prop_paramName:
      return &param_names[index].name[0];
    case ax_prop_presetData:
      return presets;
    default:
      return 0;
    }
  }

  int setProperty(ax_property_id_t id, int index, void * value) {
    switch(id) {
    case ax_prop_applyPreset: {
      ApplyPreset(index);
      return 0;
    }
    default:
      return -1;
    }
  }

  int reserved1(){return 0;}
  int reserved2(){return 0;}

    static const uint32_t nparams = 46;
    Parameter_t PExch[nparams] = {
{ type: param_type_frac_uq27, unit: param_unit_abstract, signals: 0, pfunction: parameter_function::pf_unsigned_clamp_fullrange, d: { frac: { finalvalue: 0, value: 10485760, modvalue: 10485760, offset: 0, multiplier: 0}}},
{ type: param_type_frac_uq27, unit: param_unit_abstract, signals: 0, pfunction: parameter_function::pf_unsigned_clamp, d: { frac: { finalvalue: 0, value: 124780544, modvalue: 124780544, offset: 0, multiplier: 0}}},
{ type: param_type_int, unit: param_unit_abstract, signals: 0, pfunction: 0, d: { intt: { finalvalue: 0, value: 4920, modvalue: 4920, minimum: 1000, maximum: 10000}}},
{ type: param_type_int, unit: param_unit_abstract, signals: 0, pfunction: 0, d: { intt: { finalvalue: 0, value: 176, modvalue: 176, minimum: 40, maximum: 640}}},
{ type: param_type_int, unit: param_unit_abstract, signals: 0, pfunction: 0, d: { intt: { finalvalue: 0, value: 594, modvalue: 594, minimum: 100, maximum: 2000}}},
{ type: param_type_frac_uq27, unit: param_unit_abstract, signals: 0, pfunction: parameter_function::pf_unsigned_clamp, d: { frac: { finalvalue: 0, value: 0, modvalue: 0, offset: 0, multiplier: 0}}},
{ type: param_type_frac_sq27, unit: param_unit_abstract, signals: 0, pfunction: parameter_function::pf_signed_clamp, d: { frac: { finalvalue: 0, value: 0, modvalue: 0, offset: 0, multiplier: 0}}},
{ type: param_type_frac_uq27, unit: param_unit_abstract, signals: 0, pfunction: parameter_function::pf_unsigned_clamp, d: { frac: { finalvalue: 0, value: 106954752, modvalue: 106954752, offset: 0, multiplier: 0}}},
{ type: param_type_frac_uq27, unit: param_unit_abstract, signals: 0, pfunction: parameter_function::pf_unsigned_clamp, d: { frac: { finalvalue: 0, value: 134217728, modvalue: 134217728, offset: 0, multiplier: 0}}},
{ type: param_type_frac_sq27, unit: param_unit_abstract, signals: 0, pfunction: parameter_function::pf_signed_clamp, d: { frac: { finalvalue: 0, value: -4194304, modvalue: -4194304, offset: 0, multiplier: 0}}},
{ type: param_type_frac_uq27, unit: param_unit_abstract, signals: 0, pfunction: parameter_function::pf_unsigned_clamp, d: { frac: { finalvalue: 0, value: 51380224, modvalue: 51380224, offset: 0, multiplier: 0}}},
{ type: param_type_frac_uq27, unit: param_unit_abstract, signals: 0, pfunction: parameter_function::pf_unsigned_clamp, d: { frac: { finalvalue: 0, value: 68157440, modvalue: 68157440, offset: 0, multiplier: 0}}},
{ type: param_type_frac_sq27, unit: param_unit_abstract, signals: 0, pfunction: parameter_function::pf_signed_clamp, d: { frac: { finalvalue: 0, value: 0, modvalue: 0, offset: 0, multiplier: 0}}},
{ type: param_type_frac_uq27, unit: param_unit_abstract, signals: 0, pfunction: parameter_function::pf_unsigned_clamp, d: { frac: { finalvalue: 0, value: 47185920, modvalue: 47185920, offset: 0, multiplier: 0}}},
{ type: param_type_bin_1bit_toggle, unit: param_unit_abstract, signals: 0, pfunction: 0, d: { bin: { finalvalue: 0, value: 1, modvalue: 1, nbits: 1}}},
{ type: param_type_frac_uq27, unit: param_unit_abstract, signals: 0, pfunction: parameter_function::pf_unsigned_clamp_fullrange, d: { frac: { finalvalue: 0, value: 134217728, modvalue: 134217728, offset: 0, multiplier: 0}}},
{ type: param_type_frac_sq27, unit: param_unit_abstract, signals: 0, pfunction: parameter_function::pf_signed_clamp, d: { frac: { finalvalue: 0, value: 8388608, modvalue: 8388608, offset: 0, multiplier: 0}}},
{ type: param_type_frac_sq27, unit: param_unit_abstract, signals: 0, pfunction: parameter_function::pf_signed_clamp, d: { frac: { finalvalue: 0, value: 20971520, modvalue: 20971520, offset: 0, multiplier: 0}}},
{ type: param_type_frac_sq27, unit: param_unit_abstract, signals: 0, pfunction: parameter_function::pf_signed_clamp, d: { frac: { finalvalue: 0, value: 29360128, modvalue: 29360128, offset: 0, multiplier: 0}}},
{ type: param_type_frac_sq27, unit: param_unit_abstract, signals: 0, pfunction: parameter_function::pf_signed_clamp, d: { frac: { finalvalue: 0, value: -52009369, modvalue: -52009369, offset: 0, multiplier: 0}}},
{ type: param_type_frac_uq27, unit: param_unit_abstract, signals: 0, pfunction: parameter_function::pf_unsigned_clamp, d: { frac: { finalvalue: 0, value: 36700160, modvalue: 36700160, offset: 0, multiplier: 0}}},
{ type: param_type_int, unit: param_unit_abstract, signals: 0, pfunction: 0, d: { intt: { finalvalue: 0, value: 299, modvalue: 299, minimum: 0, maximum: 40000}}},
{ type: param_type_int, unit: param_unit_abstract, signals: 0, pfunction: 0, d: { intt: { finalvalue: 0, value: 1965, modvalue: 1965, minimum: 0, maximum: 40000}}},
{ type: param_type_int, unit: param_unit_abstract, signals: 0, pfunction: 0, d: { intt: { finalvalue: 0, value: 5952, modvalue: 5952, minimum: 5000, maximum: 8000}}},
{ type: param_type_int, unit: param_unit_abstract, signals: 0, pfunction: 0, d: { intt: { finalvalue: 0, value: 1726, modvalue: 1726, minimum: 0, maximum: 8000}}},
{ type: param_type_int, unit: param_unit_abstract, signals: 0, pfunction: 0, d: { intt: { finalvalue: 0, value: 4306, modvalue: 4306, minimum: 0, maximum: 8000}}},
{ type: param_type_int, unit: param_unit_abstract, signals: 0, pfunction: 0, d: { intt: { finalvalue: 0, value: 669, modvalue: 669, minimum: 0, maximum: 8000}}},
{ type: param_type_int, unit: param_unit_abstract, signals: 0, pfunction: 0, d: { intt: { finalvalue: 0, value: 7182, modvalue: 7182, minimum: 5000, maximum: 8000}}},
{ type: param_type_int, unit: param_unit_abstract, signals: 0, pfunction: 0, d: { intt: { finalvalue: 0, value: 3210, modvalue: 3210, minimum: 0, maximum: 8000}}},
{ type: param_type_int, unit: param_unit_abstract, signals: 0, pfunction: 0, d: { intt: { finalvalue: 0, value: 5850, modvalue: 5850, minimum: 0, maximum: 8000}}},
{ type: param_type_int, unit: param_unit_abstract, signals: 0, pfunction: 0, d: { intt: { finalvalue: 0, value: 569, modvalue: 569, minimum: 0, maximum: 8000}}},
{ type: param_type_int, unit: param_unit_abstract, signals: 0, pfunction: 0, d: { intt: { finalvalue: 0, value: 4, modvalue: 4, minimum: 3, maximum: 16}}},
{ type: param_type_frac_sq27, unit: param_unit_abstract, signals: 0, pfunction: parameter_function::pf_signed_clamp, d: { frac: { finalvalue: 0, value: 134217728, modvalue: 134217728, offset: 0, multiplier: 0}}},
{ type: param_type_frac_uq27, unit: param_unit_abstract, signals: 0, pfunction: parameter_function::pf_unsigned_clamp, d: { frac: { finalvalue: 0, value: 0, modvalue: 0, offset: 0, multiplier: 0}}},
{ type: param_type_int, unit: param_unit_abstract, signals: 0, pfunction: 0, d: { intt: { finalvalue: 0, value: 3, modvalue: 3, minimum: 0, maximum: 8}}},
{ type: param_type_bin_1bit_momentary, unit: param_unit_abstract, signals: 0, pfunction: 0, d: { bin: { finalvalue: 0, value: 0, modvalue: 0, nbits: 1}}},
{ type: param_type_int, unit: param_unit_abstract, signals: 0, pfunction: 0, d: { intt: { finalvalue: 0, value: 6747, modvalue: 6747, minimum: 5000, maximum: 8000}}},
{ type: param_type_int, unit: param_unit_abstract, signals: 0, pfunction: 0, d: { intt: { finalvalue: 0, value: 426, modvalue: 426, minimum: 0, maximum: 8000}}},
{ type: param_type_int, unit: param_unit_abstract, signals: 0, pfunction: 0, d: { intt: { finalvalue: 0, value: 4758, modvalue: 4758, minimum: 0, maximum: 8000}}},
{ type: param_type_int, unit: param_unit_abstract, signals: 0, pfunction: 0, d: { intt: { finalvalue: 0, value: 3378, modvalue: 3378, minimum: 0, maximum: 8000}}},
{ type: param_type_int, unit: param_unit_abstract, signals: 0, pfunction: 0, d: { intt: { finalvalue: 0, value: 3061, modvalue: 3061, minimum: 0, maximum: 40000}}},
{ type: param_type_int, unit: param_unit_abstract, signals: 0, pfunction: 0, d: { intt: { finalvalue: 0, value: 536, modvalue: 536, minimum: 0, maximum: 40000}}},
{ type: param_type_int, unit: param_unit_abstract, signals: 0, pfunction: 0, d: { intt: { finalvalue: 0, value: 5061, modvalue: 5061, minimum: 5000, maximum: 8000}}},
{ type: param_type_int, unit: param_unit_abstract, signals: 0, pfunction: 0, d: { intt: { finalvalue: 0, value: 3194, modvalue: 3194, minimum: 0, maximum: 8000}}},
{ type: param_type_int, unit: param_unit_abstract, signals: 0, pfunction: 0, d: { intt: { finalvalue: 0, value: 194, modvalue: 194, minimum: 0, maximum: 8000}}},
{ type: param_type_int, unit: param_unit_abstract, signals: 0, pfunction: 0, d: { intt: { finalvalue: 0, value: 1611, modvalue: 1611, minimum: 0, maximum: 8000}}},
};
    Parameter_name_t param_names[nparams] = {
{ name : {'d','r','y','W','e','t','M','i',}},
{ name : {'d','r','y','W','e','t', 0 , 0 ,}},
{ name : {'t','o','n','e', 0 , 0 , 0 , 0 ,}},
{ name : {'s','p','l','i','t','L','o','w',}},
{ name : {'s','p','l','i','t','H','i','g',}},
{ name : {'g','a','i','n','L','o','w', 0 ,}},
{ name : {'b','i','a','s','L','o','w', 0 ,}},
{ name : {'l','e','v','e','l','L','o','w',}},
{ name : {'g','a','i','n','M','i','d', 0 ,}},
{ name : {'b','i','a','s','M','i','d', 0 ,}},
{ name : {'l','e','v','e','l','M','i','d',}},
{ name : {'g','a','i','n','H','i','g','h',}},
{ name : {'b','i','a','s','H','i','g','h',}},
{ name : {'l','e','v','e','l','H','i','g',}},
{ name : {'o','n','o','f','f', 0 , 0 , 0 ,}},
{ name : {'l','e','v','e','l', 0 , 0 , 0 ,}},
{ name : {'g','a','i','n','L','o','w', 0 ,}},
{ name : {'g','a','i','n','M','i','d', 0 ,}},
{ name : {'g','a','i','n','H','i','g','h',}},
{ name : {'p','i','t','c','h', 0 , 0 , 0 ,}},
{ name : {'r','e','s','o', 0 , 0 , 0 , 0 ,}},
{ name : {'t','a','p','1', 0 , 0 , 0 , 0 ,}},
{ name : {'t','a','p','2', 0 , 0 , 0 , 0 ,}},
{ name : {'w','i','d','t','h', 0 , 0 , 0 ,}},
{ name : {'t','a','p','1', 0 , 0 , 0 , 0 ,}},
{ name : {'t','a','p','2', 0 , 0 , 0 , 0 ,}},
{ name : {'t','a','p','3', 0 , 0 , 0 , 0 ,}},
{ name : {'w','i','d','t','h', 0 , 0 , 0 ,}},
{ name : {'t','a','p','1', 0 , 0 , 0 , 0 ,}},
{ name : {'t','a','p','2', 0 , 0 , 0 , 0 ,}},
{ name : {'t','a','p','3', 0 , 0 , 0 , 0 ,}},
{ name : {'f','a','d','e','S','i','z','e',}},
{ name : {'p','1', 0 , 0 , 0 , 0 , 0 , 0 ,}},
{ name : {'f','d','b','k', 0 , 0 , 0 , 0 ,}},
{ name : {'p','r','o','g','r','a','m', 0 ,}},
{ name : {'s','a','v','e', 0 , 0 , 0 , 0 ,}},
{ name : {'w','i','d','t','h', 0 , 0 , 0 ,}},
{ name : {'t','a','p','1', 0 , 0 , 0 , 0 ,}},
{ name : {'t','a','p','2', 0 , 0 , 0 , 0 ,}},
{ name : {'t','a','p','3', 0 , 0 , 0 , 0 ,}},
{ name : {'t','a','p','1', 0 , 0 , 0 , 0 ,}},
{ name : {'t','a','p','2', 0 , 0 , 0 , 0 ,}},
{ name : {'w','i','d','t','h', 0 , 0 , 0 ,}},
{ name : {'t','a','p','1', 0 , 0 , 0 , 0 ,}},
{ name : {'t','a','p','2', 0 , 0 , 0 , 0 ,}},
{ name : {'t','a','p','3', 0 , 0 , 0 , 0 ,}},
};
    int32_t displayVector[5];
    static const uint32_t ndisplay_metas = 5;
    Display_meta_t display_metas[ndisplay_metas] = {
{ display_type: display_meta_type_vu, name: {'v','u','L','e','f','t', 0 , 0 ,}, displaydata: &displayVector[0]},
{ display_type: display_meta_type_vu, name: {'v','u','R','i','g','h','t', 0 ,}, displaydata: &displayVector[1]},
{ display_type: display_meta_type_vu, name: {'v','u','L','e','f','t', 0 , 0 ,}, displaydata: &displayVector[2]},
{ display_type: display_meta_type_vu, name: {'v','u','R','i','g','h','t', 0 ,}, displaydata: &displayVector[3]},
{ display_type: display_meta_type_int32, name: {'v', 0 , 0 , 0 , 0 , 0 , 0 , 0 ,}, displaydata: &displayVector[4]},
};
    static const uint32_t NPRESETS = 8;
    static const uint32_t NPRESET_ENTRIES = 30;
    static const uint32_t NMODULATIONSOURCES = 8;
    static const uint32_t NMODULATIONTARGETS = 1;
    int32_t PExModulationPrevVal[1][NMODULATIONSOURCES];
/* modsource defines */
/* parameter instance indices */
static const int PARAM_INDEX_dryWetMixStereo__1_balance = 0;
static const int PARAM_INDEX_Distortion__1_dryWet = 1;
static const int PARAM_INDEX_Distortion__1_tone = 2;
static const int PARAM_INDEX_Distortion__1_splitLow = 3;
static const int PARAM_INDEX_Distortion__1_splitHigh = 4;
static const int PARAM_INDEX_Distortion__1_gainLow = 5;
static const int PARAM_INDEX_Distortion__1_biasLow = 6;
static const int PARAM_INDEX_Distortion__1_levelLow = 7;
static const int PARAM_INDEX_Distortion__1_gainMid = 8;
static const int PARAM_INDEX_Distortion__1_biasMid = 9;
static const int PARAM_INDEX_Distortion__1_levelMid = 10;
static const int PARAM_INDEX_Distortion__1_gainHigh = 11;
static const int PARAM_INDEX_Distortion__1_biasHigh = 12;
static const int PARAM_INDEX_Distortion__1_levelHigh = 13;
static const int PARAM_INDEX_DeluxeVerbResponse__1_onoff = 14;
static const int PARAM_INDEX_DeluxeVerbResponse__1_level = 15;
static const int PARAM_INDEX_DeluxeVerbResponse__1_gainLow = 16;
static const int PARAM_INDEX_DeluxeVerbResponse__1_gainMid = 17;
static const int PARAM_INDEX_DeluxeVerbResponse__1_gainHigh = 18;
static const int PARAM_INDEX_stereo__highpass_pitch = 19;
static const int PARAM_INDEX_stereo__highpass_reso = 20;
static const int PARAM_INDEX_diffusor__damper__tap2__2_tap1 = 21;
static const int PARAM_INDEX_diffusor__damper__tap2__2_tap2 = 22;
static const int PARAM_INDEX_delay2taps__1_width = 23;
static const int PARAM_INDEX_delay2taps__1_tap1 = 24;
static const int PARAM_INDEX_delay2taps__1_tap2 = 25;
static const int PARAM_INDEX_delay2taps__1_tap3 = 26;
static const int PARAM_INDEX_delay3taps__1_width = 27;
static const int PARAM_INDEX_delay3taps__1_tap1 = 28;
static const int PARAM_INDEX_delay3taps__1_tap2 = 29;
static const int PARAM_INDEX_delay3taps__1_tap3 = 30;
static const int PARAM_INDEX_pitchDelay__1_fadeSize = 31;
static const int PARAM_INDEX_pitchDelay__1_p1 = 32;
static const int PARAM_INDEX_pitchDelay__1_fdbk = 33;
static const int PARAM_INDEX_AkaiMidiMixReverbPlate__1_program = 34;
static const int PARAM_INDEX_AkaiMidiMixReverbPlate__1_save = 35;
static const int PARAM_INDEX_delay3taps__2_width = 36;
static const int PARAM_INDEX_delay3taps__2_tap1 = 37;
static const int PARAM_INDEX_delay3taps__2_tap2 = 38;
static const int PARAM_INDEX_delay3taps__2_tap3 = 39;
static const int PARAM_INDEX_diffusor__damper__tap2__3_tap1 = 40;
static const int PARAM_INDEX_diffusor__damper__tap2__3_tap2 = 41;
static const int PARAM_INDEX_delay2taps__2_width = 42;
static const int PARAM_INDEX_delay2taps__2_tap1 = 43;
static const int PARAM_INDEX_delay2taps__2_tap2 = 44;
static const int PARAM_INDEX_delay2taps__2_tap3 = 45;
/* object classes */
class instanceout__1{
  public: // v1
  rootc *parent;

  public: int init(rootc * _parent,
int32_t & disp_vuLeft,
int32_t & disp_vuRight) {
parent = _parent;
disp_vuLeft = 0;
disp_vuRight = 0;


 return 0;
}
  public: void dispose() {


}
  public: void dsp (rootc * parent,
const int32buffer  inlet_left,
const int32buffer  inlet_right,
int32_t & disp_vuLeft,
int32_t & disp_vuRight  ){
int j;
for(j=0;j<BUFSIZE;j++){
   AudioOutputLeft[j] += __SSAT(inlet_left[j],28);
   AudioOutputRight[j] += __SSAT(inlet_right[j],28);
}
disp_vuLeft=inlet_left[0];
disp_vuRight=inlet_right[0];

int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {

}
}
}
;class instanceoutconfig__1{
  public: // v1
  rootc *parent;

  public: int init(rootc * _parent) {
parent = _parent;
ADAU1961_WriteRegister(0x4023,(39<<2)+3);
ADAU1961_WriteRegister(0x4024,(39<<2)+3);
AudioOutputMode = A_STEREO;


 return 0;
}
  public: void dispose() {


}
  public: void dsp (rootc * parent  ){

int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {

}
}
}
;class instanceinconfig__1{
  public: // v1
  rootc *parent;

  public: int init(rootc * _parent) {
parent = _parent;
ADAU1961_WriteRegister(0x400E,(20<<2)+3);
ADAU1961_WriteRegister(0x400F,(20<<2)+3);
ADAU1961_WriteRegister(0x400B,(2<<3)+0);
ADAU1961_WriteRegister(0x400D,(2<<3)+0);
AudioInputMode = A_STEREO;


 return 0;
}
  public: void dispose() {


}
  public: void dsp (rootc * parent  ){

int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {

}
}
}
;class instancedryWetMixStereo__1{
  public: // v1
  rootc *parent;

  public: int init(rootc * _parent) {
parent = _parent;


 return 0;
}
  public: void dispose() {


}
  public: void dsp (rootc * parent,
const int32buffer  inlet_left,
const int32buffer  inlet_right,
const int32buffer  inlet_wetleft,
const int32buffer  inlet_wetright,
int32buffer  & outlet_left,
int32buffer  & outlet_right,
int param_balance  ){

int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
int32_t v = ___SMMUL(inlet_left[buffer_index], 0x7fffffff-param_balance);
v = ___SMMLA(inlet_wetleft[buffer_index], param_balance, v);
outlet_left[buffer_index]=  __SSAT(v<<1,28);

v = ___SMMUL(inlet_right[buffer_index], 0x7fffffff-param_balance);
v = ___SMMLA(inlet_wetright[buffer_index], param_balance, v);
outlet_right[buffer_index]=  __SSAT(v<<1,28);
}
}
}
;class instanceDistortion__1{
  public: // v1
  rootc *parent;
typedef struct
{
    float dryWetMix;
    float gain[3];
    float dcBias[3];
    float level[3];
    int separationFrequency[2];
    int tone;
} DistortionParameters;

class Distortion
{
public:
#ifndef M_PI_4
#define M_PI_4 (3.1415926535897932384626433832795f/4.0f)
#endif

float __attribute__ ((noinline)) FastArcTan(float x) {
  return M_PI_4*x - x*(fabs(x) - 1)*(0.2447f + 0.0663f*fabs(x));
}

	const float maxGain = 50.0f;
     Distortion()
        : m_dryWet(0.0), m_gain{0.1, 0.1, 0.1}, m_dcBias{0.0, 0.0, 0.0}
    {
        cnt = 0;
        removeDc.setHighpass(30,0.707);
        DistortionParameters initialDistortion =
            {
                1.0,{0.0,0.1,0.0},{0.0,0.0,0.0},{0.5,0.7,0.4},{70,500},5000
            };

        parameters(initialDistortion);
    }

    float __attribute__ ((noinline)) step(float in, int set)
    {
        if (m_gain[set] == 0.0)
            return in * m_level[set]*4.0;
        return atan(m_atanGain[set] * (in + m_dcBias[set])) * m_totalGain[set];
    }

    void __attribute__ ((noinline)) step(FloatBuffer &inPlace, int set)
    {
        if (m_gain[set] == 0.0)
        {
	        for (int s = 0; s < BUFSIZE; ++s)
	        {
		        inPlace[s] = inPlace[s] * m_level[set] * 4.0f;
	        }
        }
        else
        {
	        for (int s = 0; s < BUFSIZE; ++s)
	        {
	            inPlace[s] = atan(m_atanGain[set] * (inPlace[s] + m_dcBias[set])) * m_totalGain[set];
	        }
        }
    }

    void parameters(DistortionParameters &params)
    {
    		if (paramsSet.dryWetMix != params.dryWetMix)
    		{
    	        m_dryWet = params.dryWetMix;
    	        //LogTextMessage("drywet: %f", m_dryWet);
	        paramsSet.dryWetMix = params.dryWetMix;
    		}
    		
        for (int c = 0; c < 3; ++c)
        {
            m_dcBias[c] = params.dcBias[c]/2.0;
            m_level[c] = params.level[c];
		  {
	            m_gain[c] = params.gain[c]*2.0;
	            if (m_gain[c] == 0.0)
	            {
	                m_realAdjustGain[c] = 1.0;
	            } else
	            {
	                m_realAdjustGain[c] = 1.0 / atan(m_gain[c] * maxGain);
	            }
	            paramsSet.gain[c] = params.gain[c];
		  }
		  m_totalGain[c] = m_realAdjustGain[c] * m_level[c];
		  m_atanGain[c] = m_gain[c] * maxGain;
        }
        if (paramsSet.separationFrequency[0] != params.separationFrequency[0])
        {
        	   float f = params.separationFrequency[0];
        	   LogTextMessage("f=%d", (int)f);
	        lowBandFilter.setLowpass(f*2);
	        midBandFilter[0].setHighpass(f);
	        paramsSet.separationFrequency[0] = params.separationFrequency[0];
        }
        if (paramsSet.separationFrequency[1] != params.separationFrequency[1])
        {
        	   float f = paramsSet.separationFrequency[1];
        	   LogTextMessage("MB HP f=%d", (int)f);
        	   midBandFilter[1].setLowpass(f*2);
        	   upperBandFilter.setHighpass(f);
	        paramsSet.separationFrequency[1] = params.separationFrequency[1];
        }

        if (paramsSet.tone != params.tone)   
        {
        	    float f = params.tone;
	        lp.setLowpass(f, 0.5);// softer falling off
	        paramsSet.tone = params.tone;
    	   }
    }

    void update(const int32buffer inInt, int32buffer &outInt)
    {
        if (m_dryWet == 0.0)
        {
        	  memcpy(outInt, inInt, sizeof(int32buffer));
            return;
        }
        FloatBuffer in;
        for (int i = 0; i < BUFSIZE; ++i)
        {
        	in[i] = q27_to_float(inInt[i]);
        }

        FloatBuffer tmp;
        FloatBuffer lowBand;
        FloatBuffer midBand;
        FloatBuffer upperBand;
        
        lowBandFilter.update(in, lowBand, BUFSIZE);

        upperBandFilter.update(in, upperBand, BUFSIZE);
        midBandFilter[0].update(in, tmp, BUFSIZE);
        midBandFilter[1].update(tmp, midBand, BUFSIZE);
        step(lowBand, 0);
        step(midBand, 1);
        step(upperBand, 2);

        for (int i = 0; i < BUFSIZE; ++i)
        {
            tmp[i] = (lowBand[i] + midBand[i] + upperBand[i]) / 2 * m_dryWet;
        }
        FloatBuffer res;

        lp.update(tmp, res, BUFSIZE);
        removeDc.update(res, tmp,BUFSIZE);
        for (int i = 0; i < BUFSIZE; ++i)
        {
        	outInt[i] = float_to_q27(tmp[i]+in[i] * (1.0-m_dryWet));
        }

    }


private:
    AudioFilterBiquad lp;
    AudioFilterBiquad removeDc;
    AudioFilterBiquad lowBandFilter;
    AudioFilterBiquad midBandFilter[2];
    AudioFilterBiquad upperBandFilter;
    float m_dryWet;
    float m_gain[3];
    float m_level[3];
    float m_dcBias[3];
    float m_totalGain[3];
    float m_atanGain[3];
    float m_realAdjustGain[3];
    float maxvalSum;
    float minVal, maxVal;
	DistortionParameters paramsSet;
    int cnt;
};

Distortion distortion;
  public: int init(rootc * _parent) {
parent = _parent;


 return 0;
}
  public: void dispose() {


}
  public: void dsp (rootc * parent,
const int32buffer  inlet_in,
int32buffer  & outlet_out,
int param_dryWet,
int param_tone,
int param_splitLow,
int param_splitHigh,
int param_gainLow,
int param_biasLow,
int param_levelLow,
int param_gainMid,
int param_biasMid,
int param_levelMid,
int param_gainHigh,
int param_biasHigh,
int param_levelHigh  ){
static int kcnt = 0;
if (kcnt++>1000)
{
	DistortionParameters params;
	params.dryWetMix = q27_to_float(param_dryWet);
	params.gain[0] = q27_to_float(param_gainLow);
	params.gain[1] = q27_to_float(param_gainMid);
	params.gain[2] = q27_to_float(param_gainHigh);
	params.dcBias[0] = q27_to_float(param_biasLow)/2.0;
	params.dcBias[1] = q27_to_float(param_biasMid)/2.0;
	params.dcBias[2] = q27_to_float(param_biasHigh)/2.0;
	params.level[0] = q27_to_float(param_levelLow);
	params.level[1] = q27_to_float(param_levelMid);
	params.level[2] = q27_to_float(param_levelHigh);
	params.tone = (param_tone);
	params.separationFrequency[0] = (param_splitLow);
	params.separationFrequency[1] = (param_splitHigh);
	distortion.parameters(params);

	kcnt = 0;
}


distortion.update(inlet_in, outlet_out);
int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {

}
}
}
;class instanceDeluxeVerbResponse__1{
  public: // v1
  rootc *parent;
#define SAMPLE_RATE 48000

#define M_PI 3.1415926535

FilterSet<13> initial ={ // dt = 1.755837 - 'deluxe-vrb-norm-master5 Left 1x12 Right 2x12.right.csv'
0.717758, 13, {
    {LoPass, 2, EQ_NONE, 2660.5, 0, 4.761},	//  0 
    {LoPass, 2, EQ_NONE, 7525.04, 0, 13.689},	//  1 
    {LoPass, 2, EQ_NONE, 3986.25, 0, 5.625},	//  2 
    {HiPass, 1, EQ_NONE, 48.493, 0, 1.089},	//  3 
    {LoShelf, 1, EQ_NONE, 822.025, -0.8, 5.776},	//  4 
    {HiShelf, 1, EQ_NONE, 5747.01, 2.6, 38.025},	//  5 
    {Peak, 1, EQ_NONE, 4832.64, 2.44, 0.4},	//  6 
    {Peak, 1, EQ_NONE, 1280, -1.4, 0.9},	//  7 
    {Notch, 1, EQ_NONE, 3103.55, 0, 0.5},	//  8 
    {Peak, 1, EQ_NONE, 71.2719, 4.5, 9.604},	//  9 

    {LoShelf, 1, EQ_LOW, 120, 0.0, 0.707},	
    {Peak, 1, EQ_MID, 500, 0.0, 0.707},
    {HiShelf, 1, EQ_HIGH, 3000, 0.0, 0.707},	
}};

FilterChain<13,0> ampChain;
  public: int init(rootc * _parent) {
parent = _parent;
ampChain.setParams(initial);

 return 0;
}
  public: void dispose() {


}
  public: void dsp (rootc * parent,
const int32buffer  inlet_in,
int32buffer  & outlet_out,
int param_onoff,
int param_level,
int param_gainLow,
int param_gainMid,
int param_gainHigh  ){
static int krateCount=2990;
if (krateCount++ >= 1000)
{
	ampChain.setEQ(q27_to_float(param_gainLow), q27_to_float(param_gainMid), q27_to_float(param_gainHigh));
	krateCount = 0;
//	krate30(parameters);
}


int32buffer result;
int32buffer feed;
int32buffer dry;

if (param_onoff)
	ampChain.dsp(inlet_in, outlet_out);
else
	memcpy(outlet_out, inlet_in, sizeof(dry));
int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {

}
}
}
;class instancepredelay__slap{
  public: // v1
  rootc *parent;
const int32_t maxSize = 48000*256/1000; // max 200msecs
int32_t *d;
int32_t head;
int32_t tail;
  public: int init(rootc * _parent) {
parent = _parent;
d=(int32_t *) sdram_malloc(sizeof(int32_t)*maxSize);
memset(d,0, sizeof(int32_t)*maxSize);

head = 0;

 return 0;
}
  public: void dispose() {
ax_free(d);

}
  public: void dsp (rootc * parent,
const int32buffer  inlet_in,
const int32_t inlet_msecs,
int32buffer  & outlet_out  ){

int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
outlet_out[buffer_index] = d[tail++];
d[head++] = inlet_in[buffer_index];

if (head >= maxSize)
{
	head = 0;
	tail = maxSize - (inlet_msecs*48+1);
}

if (tail >= maxSize)
{
	tail = 0;
}
}
}
}
;class instanceinput__diffusor__2{
  public: // v1
  rootc *parent;
class FeedbackRing
{

public:
	FeedbackRing()
	: m_size(0)
	, m_head(0)
	, m_gain(0)
	, m_buffer(0)
	, m_adjustGain(false)
	{
		
	}

	void reset()
	{
		memset(m_buffer,0, sizeof(int32_t)*m_size);
		m_head = 0;
		m_adjustGain = false;	
	}

	void initSram(int size, int32_t *buffer)
	{
		m_buffer = buffer;
		m_size = size;
		reset();
	}

	void init(int size)
	{
		m_buffer = (int32_t *) sdram_malloc(sizeof(int32_t)*size);
		m_size = size;
		reset();
	}

	void gain(int32_t gain_q27, bool adjustGain=false)
	{
		if (float_to_q27(0.9) <= gain_q27)
			gain_q27 = float_to_q27(0.9);
		m_gain = gain_q27<<3;
		m_outgain = 8<<27;
		if (adjustGain)
		{
			adjustOutputGain();
		}
		// LogTextMessage("%f %f", q27_to_float(m_gain)/8.0, q27_to_float(m_outgain)/8.0);
	}

	void adjustOutputGain()
	{
		const float cx=1.0, cy=0.35, cd=0.73;
		float currentAdjust = 1.0/((1.0f / (cx-fabs(q27_to_float(m_gain>>3)))) * cd + cy);
		m_outgain = float_to_q27(currentAdjust)<<3;
		m_adjustGain = true;
	}

	void setDamping(int32_t damping)
	{
		m_damper = damping;
	}
	
	int32_t step(int32_t in)
	{
		int32_t ret = ___SMMLA(in<<2, m_gain, m_buffer[m_head]);
		int32_t satValue = ___SMMLA(m_buffer[m_head]<<2, -m_gain, in); // N.B. negativ feedback
		int32_t delta = prev-satValue;
		offset =___SMMUL(m_damper<<3,offset<<2)+delta;
		prev = satValue;
		m_buffer[m_head++] = offset+satValue;
		
		if (m_head >= m_size)
			m_head = 0;
		if (m_adjustGain)
		{
			ret = ___SMMUL(ret, m_outgain)<<2;
		}
		return ret;
	}
	
	int32_t tap(int32_t position)
	{
		return m_buffer[position];
	}

private:
	int32_t m_size;
	int32_t m_head;
	int32_t m_gain;
	int32_t m_outgain;
	bool m_adjustGain;
	int32_t *m_buffer;
	int32_t m_damper;
	int32_t prev;
	int32_t offset;
};
FeedbackRing rng[4];
  public: int init(rootc * _parent) {
parent = _parent;
static int32_t sram_buffer1[217];
static int32_t sram_buffer2[173];
static int32_t sram_buffer3[587];
static int32_t sram_buffer4[439];


rng[0].initSram(217, sram_buffer1);
rng[1].initSram(173, sram_buffer2);
rng[2].initSram(587, sram_buffer3);
rng[3].initSram(439, sram_buffer4);

 return 0;
}
  public: void dispose() {


}
  public: void dsp (rootc * parent,
const int32buffer  inlet_in,
const int32_t  inlet_fdbk,
const int32_t  inlet_damping,
int32buffer  & outlet_out  ){
static int32_t oldFdbk=123;

if (oldFdbk!=inlet_fdbk)
{
	oldFdbk = inlet_fdbk;
	rng[0].gain(inlet_fdbk);
	rng[1].gain(inlet_fdbk, true);

	int32_t a = inlet_fdbk*5/6;
	rng[2].gain(a);
	rng[3].gain(a, true);
}

static int32_t oldDamping=123;
if (oldDamping!=inlet_damping)
{
	oldDamping = inlet_damping;
	for (int c=0; c < 4; ++c)
	{
		rng[c].setDamping(inlet_damping);
	}
}
int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
int32_t value = inlet_in[buffer_index];

value = rng[0].step(value);
value = rng[1].step(value);
value = rng[2].step(value);
value = rng[3].step(value);

outlet_out[buffer_index] = value;
}
}
}
;class instancein__1{
  public: // v1
  rootc *parent;

  public: int init(rootc * _parent,
int32_t & disp_vuLeft,
int32_t & disp_vuRight) {
parent = _parent;
disp_vuLeft = 0;
disp_vuRight = 0;


 return 0;
}
  public: void dispose() {


}
  public: void dsp (rootc * parent,
int32buffer  & outlet_left,
int32buffer  & outlet_right,
int32_t & disp_vuLeft,
int32_t & disp_vuRight  ){
int j;
for(j=0;j<BUFSIZE;j++){
   outlet_left[j] = AudioInputLeft[j];
   outlet_right[j] = AudioInputRight[j];
}
disp_vuLeft=outlet_left[0];
disp_vuRight=outlet_right[0];

int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {

}
}
}
;class instancelong__time__decay__feedback{
  public: // v1
  rootc *parent;

  public: int init(rootc * _parent) {
parent = _parent;


 return 0;
}
  public: void dispose() {


}
  public: void dsp (rootc * parent,
const int32buffer  inlet_mono,
const int32buffer  inlet_left,
const int32buffer  inlet_right,
const int32_t  inlet_feedback,
int32buffer  & outlet_left,
int32buffer  & outlet_right  ){

int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
int32_t accum = ___SMMUL(inlet_left[buffer_index]<<2, inlet_feedback<<3);

outlet_left[buffer_index] =  __SSAT(inlet_mono[buffer_index] + accum, 28);
accum = ___SMMUL(inlet_right[buffer_index]<<2, inlet_feedback<<3);
outlet_right[buffer_index] =  __SSAT(inlet_mono[buffer_index] + accum, 28);
}
}
}
;class instanceharpReso__1{
  public: // v1
  rootc *parent;
class FeedbackRing2
{

public:
	FeedbackRing2()
	: m_size(0)
	, m_head(0)
	, m_gain(0)
	, m_buffer(0)
	, m_adjustGain(false)
	{
		
	}

	void reset()
	{
		memset(m_buffer,0, sizeof(int32_t)*m_size);
		m_head = 0;
		m_adjustGain = false;	
	}

	void initSram(int size, int32_t *buffer)
	{
		m_buffer = buffer;
		m_size = size;
		reset();
	}

	void init(int size)
	{
		m_buffer = (int32_t *) sdram_malloc(sizeof(int32_t)*size);
		m_size = size;
		reset();
	}

	void gain(int32_t gain_q27, bool adjustGain=false)
	{
		if (float_to_q27(0.995) <= gain_q27)
			gain_q27 = float_to_q27(0.995);
		m_gain = gain_q27<<3;
		m_outgain = 8<<27;
		if (adjustGain)
		{
			adjustOutputGain();
		}
		// LogTextMessage("%f %f", q27_to_float(m_gain)/8.0, q27_to_float(m_outgain)/8.0);
	}

	void adjustOutputGain()
	{
		const float cx=1.0, cy=0.35, cd=0.73;
		float currentAdjust = 1.0/((1.0f / (cx-fabs(q27_to_float(m_gain>>3)))) * cd + cy);
		m_outgain = float_to_q27(currentAdjust)<<3;
		m_adjustGain = true;
	}

	void setDamping(int32_t damping)
	{
		m_damper = damping;
	}
	
	int32_t step(int32_t in)
	{
		int32_t ret = ___SMMLA(in<<2, m_gain, m_buffer[m_head]);
		int32_t satValue = ___SMMLA(m_buffer[m_head]<<2, -m_gain, in); // N.B. negativ feedback
		int32_t delta = prev-satValue;
		offset =___SMMUL(m_damper<<3,offset<<2)+delta;
		prev = satValue;
		m_buffer[m_head++] = offset+satValue;
		
		if (m_head >= m_size)
			m_head = 0;
		if (m_adjustGain)
		{
			ret = ___SMMUL(ret, m_outgain)<<2;
		}
		return ret;
	}
	
	int32_t tap(int32_t position)
	{
		return m_buffer[position];
	}

private:
	int32_t m_size;
	int32_t m_head;
	int32_t m_gain;
	int32_t m_outgain;
	bool m_adjustGain;
	int32_t *m_buffer;
	int32_t m_damper;
	int32_t prev;
	int32_t offset;
};
FeedbackRing2 rng[4];
  public: int init(rootc * _parent) {
parent = _parent;
#define MAXSIZE 10
static int32_t sram_buffer1[MAXSIZE];
static int32_t sram_buffer2[MAXSIZE];
static int32_t sram_buffer3[MAXSIZE];
static int32_t sram_buffer4[MAXSIZE];


rng[0].initSram(MAXSIZE, sram_buffer1);
rng[1].initSram(MAXSIZE, sram_buffer2);
rng[2].initSram(MAXSIZE, sram_buffer3);
rng[3].initSram(MAXSIZE, sram_buffer4);

 return 0;
}
  public: void dispose() {


}
  public: void dsp (rootc * parent,
const int32buffer  inlet_in,
const int32_t  inlet_fdbk,
const int32_t  inlet_damping,
int32buffer  & outlet_out  ){
static int32_t oldFdbk=123;

if (oldFdbk!=inlet_fdbk)
{
	oldFdbk = inlet_fdbk;
	for (int c=0; c < 4; ++c)
		rng[c].gain(inlet_fdbk, true);
}

static int32_t oldDamping=123;
if (oldDamping!=inlet_damping)
{
	oldDamping = inlet_damping;
	for (int c=0; c < 4; ++c)
	{
		rng[c].setDamping(inlet_damping);
	}
}
int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
int32_t value = rng[0].step(inlet_in[buffer_index]);

value += rng[1].step(inlet_in[buffer_index]);
value += rng[2].step(inlet_in[buffer_index]);
value += rng[3].step(inlet_in[buffer_index]);

outlet_out[buffer_index] = __SSAT(value>>2, 28);
}
}
}
;class instancestereo__highpass{
  public: // v1
  rootc *parent;
biquad_coefficients bc;
biquad_state bs[2];
  public: int init(rootc * _parent) {
parent = _parent;
biquad_clearstate(&bs[0]);
biquad_clearstate(&bs[1]);

 return 0;
}
  public: void dispose() {


}
  public: void dsp (rootc * parent,
const int32buffer  inlet_left,
const int32buffer  inlet_right,
int32buffer  & outlet_left,
int32buffer  & outlet_right,
int param_pitch,
int param_reso  ){
static int32_t oldPitch=-1, oldReso=-1;

if ((oldPitch != param_pitch) || (oldReso != param_reso))
{ // only recalculate if changed 
	int32_t freq;
	MTOF(param_pitch,freq);
	biquad_hp_coefs(&bc,freq,INT_MAX - (__USAT(param_reso,27)<<4));
	oldPitch = param_pitch;
	oldReso = param_reso;
}

biquad_dsp(&bs[0],&bc,inlet_left,outlet_left);
biquad_dsp(&bs[1],&bc,inlet_right,outlet_right);
int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {

}
}
}
;class instancediffusor__damper__tap2__2{
  public: // v1
  rootc *parent;
class FeedbackRing
{
public:
	FeedbackRing()
	: m_size(0)
	, m_head(0)
	, m_gain(0)
	, prev(0)
	, offset(0)
	{
	}

	~FeedbackRing()
	{
		if (m_buffer)
			ax_free(m_buffer);	
	}

	void dispose()
	{
		if (m_buffer)
			ax_free(m_buffer);	
		m_buffer = 0;
	}

	void init(int size)
	{
		m_buffer = (int32_t *) sdram_malloc(size*sizeof(int32_t));
		LogTextMessage("diffuser mem: %d", size);
		m_size = size;
		memset(m_buffer, 0, sizeof(int32_t)*m_size);
		m_head = 0;
		m_adjustGain = false;
		prev = 0;
		offset = 0;
	}

	void gain(int32_t gain_q27, bool adjustGain=false)
	{
		if (float_to_q27(0.97) <= gain_q27)
			gain_q27 = float_to_q27(0.97);
		m_gain = gain_q27<<3;
		m_outgain = 8<<27;
		if (adjustGain)
		{
			adjustOutputGain();
		}
	}

	void setDamping(int32_t damping)
	{
		m_damper = damping;
	}

	void adjustOutputGain()
	{
		const float cx=1.0, cy=0.35, cd=0.73;
		float currentAdjust = 1.0/((1.0f / (cx-fabs(q27_to_float(m_gain>>3)))) * cd + cy);
		m_outgain = float_to_q27(currentAdjust)<<3;
		m_adjustGain = true;

	}

	
	int32_t tap(int32_t rel_position)
	{
		int h=m_head-rel_position+m_size;
		if (h >= m_size)
			h-=m_size;
		return m_buffer[h];
	}

	int32_t update(const int32buffer in, int32buffer &out, int32_t tap1Pos, int32buffer &tap1, int32_t tap2Pos,int32buffer &tap2)
	{
		
		int head = m_head;
		for (int i=0; i < BUFSIZE; ++i)
		{
			int32_t ret = ___SMMLA(in[i]<<2, m_gain, m_buffer[m_head]);
			int32_t satValue = ___SMMLA(m_buffer[m_head]<<2, -m_gain, in[i]); // N.B. negativ feedback
			int32_t delta = prev-satValue;
			offset =___SMMUL(m_damper<<3,offset<<2)+delta;
			prev = satValue;
		
			m_buffer[m_head++] = offset+satValue;
			if (m_head >= m_size)
				m_head = 0;
			if (m_adjustGain)
			{
				ret = ___SMMUL(ret, m_outgain)<<2;
			}
			out[i] = ret;
			tap1[i] = tap(tap1Pos);
			tap2[i] = tap(tap2Pos);
		}
	}

private:
	int32_t m_size;
	int32_t m_head;
	int32_t m_gain;
	int32_t m_outgain;
	bool m_adjustGain;
	int32_t *m_buffer;
     int32_t m_damper;
	int32_t prev;
	int32_t offset;
};

FeedbackRing rng;
  public: int init(rootc * _parent) {
parent = _parent;
rng.init(2903);

 return 0;
}
  public: void dispose() {
rng.dispose();

}
  public: void dsp (rootc * parent,
const int32buffer  inlet_in,
const int32_t  inlet_fdbk,
const int32_t  inlet_damping,
int32buffer  & outlet_out,
int32buffer  & outlet_tap1,
int32buffer  & outlet_tap2,
int param_tap1,
int param_tap2  ){
static int32_t oldFdbk=123;

if (oldFdbk != inlet_fdbk)
{
	oldFdbk = inlet_fdbk;
	rng.gain(oldFdbk, true);
}

rng.setDamping(inlet_damping);

rng.update(inlet_in, outlet_out, param_tap1, outlet_tap1, param_tap2, outlet_tap2);
int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {

}
}
}
;class instancedelay2taps__1{
  public: // v1
  rootc *parent;
const int32_t maxSize = 8192;
int32_t *d;
int32_t head;
int32_t tap1, tap2, tap3;
  public: int init(rootc * _parent) {
parent = _parent;
d=(int32_t *) sdram_malloc(sizeof(int32_t)*maxSize);
memset(d,0, sizeof(int32_t)*maxSize);

head = 0;
tap1 = 0;
tap2 = 0;
tap3 = 0;

 return 0;
}
  public: void dispose() {


}
  public: void dsp (rootc * parent,
const int32buffer  inlet_in,
int32buffer  & outlet_out,
int32buffer  & outlet_tap1,
int32buffer  & outlet_tap2,
int32buffer  & outlet_tap3,
int param_width,
int param_tap1,
int param_tap2,
int param_tap3  ){

int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
outlet_out[buffer_index] = d[head];
d[head++] = inlet_in[buffer_index];

if (head >= param_width)
{
	head = 0;
	tap1 = param_tap1;
	tap2 = param_tap2;
	tap3 = param_tap3;
}

outlet_tap1[buffer_index] = d[tap1];
tap1++;
if (tap1 >= param_width)
	tap1 = 0;

outlet_tap2[buffer_index] = d[tap2];
tap2++;
if (tap2 >= param_width)
	tap2 = 0;

outlet_tap3[buffer_index] = d[tap3];
tap3++;
if (tap3 >= param_width)
	tap3 = 0;
}
}
}
;class instancemodFdbkDelay__1{
  public: // v1
  rootc *parent;
int32_t *d;
int32_t head=0;
int32_t tail=2;
int32_t currentWidth;
int32_t currentPhase=0;
int32_t krateStep=0;
int32_t damper;
int32_t prev;
int32_t offset;
  public: int init(rootc * _parent) {
parent = _parent;
static int32_t sram_buffer1[1217];
d = sram_buffer1;
head = 0;

 return 0;
}
  public: void dispose() {


}
  public: void dsp (rootc * parent,
const int32buffer  inlet_in,
const int32_t  inlet_fdbk,
const int32_t  inlet_damper,
const int32_t inlet_modulationTime,
const int32_t inlet_moddepth,
int32buffer  & outlet_out  ){
krateStep++;

if (krateStep < inlet_modulationTime)
{
	currentPhase=inlet_moddepth*krateStep/inlet_modulationTime;
}
else if (krateStep < inlet_modulationTime*2)
{
	currentPhase=inlet_moddepth*(inlet_modulationTime*2-krateStep)/inlet_modulationTime;
}
else
{
	krateStep = 0;
}

damper = inlet_damper;

int32_t fdbk = inlet_fdbk<<3; 

tail = head+2+currentPhase;

if (tail>= 1217)
	tail-=1217;
int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
/*
  out =  delay+in*factor
  delayIn = delay*factor + in
 */

outlet_out[buffer_index] = ___SMMLA(-inlet_in[buffer_index]<<2, fdbk, d[tail]);
int32_t valueD = ___SMMLA(d[tail]<<2, fdbk, inlet_in[buffer_index]);

tail++;
if (tail >= 1217)
{
	tail = 0;
}

int32_t delta = prev-valueD;
offset =___SMMUL(damper<<3,offset<<2)+delta;
prev = valueD;

d[head++] = offset+prev;

if (head >= 1217)
{
	head = 0;
}
}
}
}
;class instancedelay3taps__1{
  public: // v1
  rootc *parent;
const int32_t maxSize = 8192;
int32_t *d;
int32_t head;
int32_t tap1, tap2, tap3;
  public: int init(rootc * _parent) {
parent = _parent;
d=(int32_t *) sdram_malloc(sizeof(int32_t)*maxSize);
memset(d, 0, sizeof(int32_t)*maxSize);
head = 0;
tap1 = 0;
tap2 = 0;
tap3 = 0;

 return 0;
}
  public: void dispose() {
ax_free(d);

}
  public: void dsp (rootc * parent,
const int32buffer  inlet_in,
int32buffer  & outlet_out,
int32buffer  & outlet_tap1,
int32buffer  & outlet_tap2,
int32buffer  & outlet_tap3,
int param_width,
int param_tap1,
int param_tap2,
int param_tap3  ){

int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
outlet_out[buffer_index] = d[head];
d[head++] = inlet_in[buffer_index];

if (head >= param_width)
{
	head = 0;
	tap1 = param_tap1;
	tap2 = param_tap2;
	tap3 = param_tap3;
}

outlet_tap1[buffer_index] = d[tap1];
tap1++;
if (tap1 >= param_width)
	tap1=0;

outlet_tap2[buffer_index] = d[tap2];
tap2++;
if (tap2 >= param_width)
	tap2=0;

outlet_tap3[buffer_index] = d[tap3];
tap3++;
if (tap3 >= param_width)
	tap3=0;
}
}
}
;class instancedownmix{
  public: // v1
  rootc *parent;

  public: int init(rootc * _parent) {
parent = _parent;


 return 0;
}
  public: void dispose() {


}
  public: void dsp (rootc * parent,
const int32buffer  inlet_p__in1,
const int32buffer  inlet_p__in2,
const int32buffer  inlet_n__in3,
const int32buffer  inlet_p__in4,
const int32buffer  inlet_n__in5,
const int32buffer  inlet_n__in6,
const int32buffer  inlet_n__in7,
const int32buffer  inlet_p__in8,
int32buffer  & outlet_out  ){

int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
int32_t a = inlet_p__in1[buffer_index];

a += inlet_p__in2[buffer_index];
a -= inlet_n__in3[buffer_index];
a += inlet_p__in4[buffer_index];
a -= inlet_n__in5[buffer_index];
a -= inlet_n__in6[buffer_index];
a -= inlet_n__in7[buffer_index];
a += inlet_p__in8[buffer_index];
outlet_out[buffer_index] = a;
}
}
}
;class instancepitchDelay__1{
  public: // v1
  rootc *parent;
#include "/Users/scjurgen/Desktop/axoloti-guitar-amp/PitchDelay.h"

PitchDelay oDelay[2];


void init(int size)
{
	oDelay[0].init_delay(size);
	oDelay[1].init_delay(size);
}
  public: int init(rootc * _parent) {
parent = _parent;
init(8192);

 return 0;
}
  public: void dispose() {


}
  public: void dsp (rootc * parent,
const int32buffer  inlet_inLeft,
const int32buffer  inlet_inRight,
const int32_t  inlet_feedback,
const int32_t  inlet_damping,
const int32_t  inlet_ratioLeft,
const int32_t  inlet_ratioRight,
const int32_t  inlet_drywet,
int32buffer  & outlet_outLeft,
int32buffer  & outlet_outRight,
int param_fadeSize,
int param_p1,
int param_fdbk  ){
static int krateCount=2990;
krateCount++;
if (krateCount==3000)
{
	krateCount = 0;
	oDelay[0].setFadeTime(param_fadeSize);
	oDelay[1].setFadeTime(param_fadeSize);
	oDelay[0].setPitch(inlet_ratioLeft);
	oDelay[1].setPitch(inlet_ratioRight);
	oDelay[0].setFeedback(inlet_feedback);
	oDelay[1].setFeedback(inlet_feedback);
//	oDelay[0].setFeedback(param_fdbk);
//	oDelay[1].setFeedback(param_fdbk);
	oDelay[0].setDamping(inlet_damping);
	oDelay[1].setDamping(inlet_damping);
}
int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
int32_t value = oDelay[0].step(inlet_inLeft[buffer_index]);
//outlet_outLeft[buffer_index] = value;
int32_t v = ___SMMUL(inlet_inLeft[buffer_index], (0x7ffffff-inlet_drywet)<<4);
v = ___SMMLA(value, inlet_drywet<<4, v);
outlet_outLeft[buffer_index]=  __SSAT(v<<1,28);

value = oDelay[1].step(inlet_inRight[buffer_index]);
//outlet_outRight[buffer_index] = value;
v = ___SMMUL(inlet_inRight[buffer_index],(0x7ffffff-inlet_drywet)<<4);
v = ___SMMLA(value, inlet_drywet<<4, v);
outlet_outRight[buffer_index]=  __SSAT(v<<1,28);
}
}
}
;class instancei__2{
  public: // v1
  rootc *parent;

  public: int init(rootc * _parent,
int32_t & disp_v) {
parent = _parent;
disp_v = 0;


 return 0;
}
  public: void dispose() {


}
  public: void dsp (rootc * parent,
const int32_t inlet_in,
int32_t & disp_v  ){
disp_v=inlet_in;

int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {

}
}
}
;class instanceAkaiMidiMixReverbPlate__1{
  public: // v1
  rootc *parent;
/*
import math
def getRatio(x:float):
    return math.pow(math.pow(2,1/12),x)
for i in range(-12,14):
    for cents in range(-2,3):
        idx = (i+12)*5+(cents+2)
        print("{v}, // {idx}".format(idx=idx, v=getRatio(i+cents/10.0)))
*/

/* this maps 128 values to a staircase function that is composed like 
 *  -20cents, -10cents, 0, +10,+20, 
 *  for 26 tones
 */
const float mapRatio[128]=
{
0.49425701017644785, // 0
0.4971202119087734, // 1
0.4999999999999997, // 2 -1 Octave
0.5028964705339265, // 3
0.5058097201509609, // 4
0.5236470614103131, // 5
0.5266805179774177, // 6
0.5297315471796474, // 7 11 down
0.5328002508142419, // 8
0.5358867312681463, // 9
0.5547847360339223, // 10
0.5579985713151114, // 11
0.5612310241546863, // 12 10 down
0.5644822024030653, // 13
0.5677522145354384, // 14
0.5877739531418041, // 15
0.5911788930135738, // 16
0.5946035575013603, // 17 9 down
0.5980480608687017, // 18
0.6015125180410581, // 19
0.6227248111794113, // 20
0.6263322193120637, // 21
0.6299605249474364, // 22 8 down
0.6336098491435309, // 23
0.6372803136596309, // 24
0.6597539553864469, // 25
0.6635758711692838, // 26
0.667419927085017, // 27  7 Quint down
0.6712862513901315, // 28
0.6751749730840948, // 29
0.6989849670895095, // 30
0.7030341458111745, // 31
0.7071067811865474, // 32 6
0.7112030090986405, // 33
0.715322966217629, // 34
0.740548776143282, // 35
0.7448387315613509, // 36
0.7491535384383405, // 37 5 Quart down
0.7534933407372878, // 38
0.7578582832551989, // 39
0.7845840978967505, // 40
0.7891291473386415, // 41
0.7937005259840996, // 42 4 
0.7982983863566497, // 43
0.8029228818633763, // 44
0.8312378961427876, // 45
0.8360532082885093, // 46
0.8408964152537144, // 47 3
0.8457676786314484, // 48
0.8506671609508556, // 49
0.8806658735961483, // 50
0.8857675191023605, // 51
0.8908987181403393, // 52 2 semitones down
0.8960596419119524, // 53
0.9012504626108302, // 54
0.9330329915368074, // 55
0.938437996671143, // 56
0.9438743126816934, // 57 1 semitone down
0.9493421209505192, // 58
0.9548416039104165, // 59
0.9885140203528962, // 60
0.9942404238175473, // 61
1.0000000000000000, // 62 no change
1.0057929410678534, // 63 +10 cents detune
1.0116194403019225, // 64 
1.0472941228206267, // 65
1.053361035954836, // 66
1.0594630943592953, // 67 1 semitone up
1.0656005016284844, // 68
1.0717734625362931, // 69
1.109569472067845, // 70
1.1159971426302233, // 71
1.122462048309373, // 72   2 semitones up
1.1289644048061314, // 73
1.1355044290708776, // 74
1.175547906283609, // 75
1.1823577860271484, // 76
1.1892071150027212, // 77 3 minor 3rd
1.196096121737404, // 78
1.2030250360821169, // 79
1.2454496223588232, // 80
1.2526644386241281, // 81
1.2599210498948734, // 82 4 major 3rd up
1.2672196982870625, // 83 
1.2745606273192625, // 84
1.3195079107728944, // 85
1.3271517423385684, // 86
1.3348398541700346, // 87 5 quart up
1.3425725027802637, // 88
1.3503499461681903, // 89
1.3979699341790197, // 90
1.40606829162235, // 91
1.4142135623730954, // 92 6 tritone up
1.4224060181972817, // 93
1.4306459324352587, // 94
1.4810975522865646, // 95
1.4896774631227028, // 96
1.498307076876682, // 97  7 a neat 5th up
1.5069866814745763, // 98
1.5157165665103987, // 99
1.569168195793502, // 100
1.5782582946772836, // 101
1.5874010519682, // 102     8 semis up
1.5965967727133004, // 103
1.6058457637267534, // 104
1.6624757922855762, // 105
1.6721064165770194, // 106
1.6817928305074297, // 107 9 semis up
1.6915353572628977, // 108
1.701334321901712, // 109
1.7613317471922978, // 110
1.771535038204722, // 111
1.7817974362806794, // 112 10 semis (7th)
1.7921192838239057, // 113
1.8025009252216613, // 114
1.8660659830736157, // 115
1.8768759933422872, // 116
1.887748625363388, // 117 11 semis (maj 7th)
1.8986842419010392, // 118
1.909683207820834, // 119
1.9770280407057934, // 120
1.9884808476350957, // 121
2.000000000000001, // 122  octave up
2.0115858821357078, // 123
2.023238880603846, // 124
2.094588245641255, // 125
2.106722071909673, // 126
2.1189261887185915, // 127
};


struct DataSet
{
     int32_t modulationFeedback;
     int32_t modulationDamp;
     int32_t modulationTime;
     int32_t modulationDepth;
     int32_t modulationTime2;
     int32_t modulationDepth2;
     
     int32_t inputDiffusorFeedback;
     int32_t inputDiffusorDamping;
     
     int32_t tankFeedback;
     
     int32_t diffusorFeedback;
     int32_t diffusorDamping;
     
     int32_t predelayMsecs;
     
     int32_t pitchFeedback;
     int32_t pitchDamping;
     int32_t pitchRatioLeft;
     int32_t pitchRatioRight;
     int32_t pitchMix;
     
};

DataSet dataSet = 
{
	0, 0, 1900, 60, 2000,60,
	0, 0, 
	0, 
	0, 0, 
	100,
	0,0,0x7ffffff,0x7ffffff,0x2000000,
};

const DataSet reverbPrograms[8] =
{
	// tight room
    {0x04100000, 0x07500000, 0x000004E0, 0x0000002E, 0x000005C0, 0x00000000, 
	0x01100000, 0x00000000,
	0x00000000,
	0x04100000, 0x07800000,
	0x00000000,
	0x00000000, 0x00000000, 0x0FD0F410,0x0FE868A0,0x00000000},
	
	// small medium bright
{0x04100000, 0x05D00000, 0x000004E0, 0x0000002E, 0x000005C0, 0x00000000, 
	0x06500000, 0x01B00000,
	0x00000000,
	0x04100000, 0x05700000,
	0x00000000,
	0x00000000, 0x00000000, 0x0FD0F410,0x0FE868A0,0x00000000},

	// bigger brighter
{0x05500000, 0x02800000, 0x000004E0, 0x0000002E, 0x000005C0, 0x00000000, 
	0x06000000, 0x00000000,
	0x00000000,
	0x05500000, 0x02700000,
	0x00000012,
	0x00000000, 0x00000000, 0x0FD0F410,0x0FE868A0,0x00000000},

{0x05500000, 0x00000000, 0x000004E0, 0x00000020, 0x000005C0, 0x00000000, 
	0x06000000, 0x00000000,
	0x00000000,
	0x05500000, 0x00000000,
	0x00000088,
	0x02D00000, 0x04600000, 0x08000000,0x080BDD30,0x07F00000},

{0x04000000, 0x00000000, 0x000004E0, 0x00000076, 0x00000580, 0x00000000, 
	0x05F00000, 0x00000000,
	0x03700000,
	0x05C00000, 0x04600000,
	0x00000026,
	0x00000000, 0x00000000, 0x0BFC8870,0x0556E040,0x03300000},
	// secret society choir
{0x05500000, 0x00000000, 0x000000D0, 0x0000001E, 0x00000580, 0x00000000, 
	0x06000000, 0x00000000,
	0x05D00000,
	0x05700000, 0x00000000,
	0x00000028,
	0x00200000, 0x05500000, 0x10000000,0x040BE5F8,0x01100000},
	// organ style
	
	{0x00000000, 0x00000000, 0x000000D0, 0x0000001E, 0x00000580, 0x00000000, 
	0x00000000, 0x00000000,
	0x05D00000,
	0x00000000, 0x00000000,
	0x00000028,
	0x03200000, 0x00700000, 0x0C0E4F10,0x0BFC8870,0x02400000},
	// long reverb
{0x05800000, 0x00000000, 0x000002F0, 0x0000000E, 0x00000580, 0x00000000, 
	0x05A00000, 0x00000000,
	0x05B00000,
	0x05400000, 0x00000000,
	0x000000B8,
	0x05700000, 0x04000000, 0x08000000,0x080BDD30,0x04900000},
};

void printDataSet()
{
	LogTextMessage("{0x%08x, 0x%08x, 0x%08x, 0x%08x, 0x%08x, 0x%08x, ",dataSet.modulationFeedback, dataSet.modulationDamp,
                                                         dataSet.modulationTime,
                                                         dataSet.modulationDepth,
                                                         dataSet.modulationTime2,
                                                         dataSet.modulationDepth2);
     LogTextMessage("\t0x%08x, 0x%08x,", dataSet.inputDiffusorFeedback, dataSet.inputDiffusorDamping);
     LogTextMessage("\t0x%08x,", dataSet.tankFeedback);
     LogTextMessage("\t0x%08x, 0x%08x,", dataSet.diffusorFeedback, dataSet.diffusorDamping);
     LogTextMessage("\t0x%08x,", dataSet.predelayMsecs);
     LogTextMessage("\t0x%08x, 0x%08x, 0x%08x,0x%08x,0x%08x},", dataSet.pitchFeedback, dataSet.pitchDamping,
        dataSet.pitchRatioLeft,
        dataSet.pitchRatioRight,
        dataSet.pitchMix);
}
  public: int init(rootc * _parent) {
parent = _parent;


 return 0;
}
  public: void dispose() {


}
  public: void dsp (rootc * parent,
int32_t  & outlet_modulationFeedback,
int32_t  & outlet_modulationDamp,
int32_t & outlet_modulationTime,
int32_t & outlet_modulationTime2,
int32_t & outlet_modulationDepth,
int32_t & outlet_modulationDepth2,
int32_t  & outlet_inputDiffusorFeedback,
int32_t  & outlet_inputDiffusorDamping,
int32_t  & outlet_tankFeedback,
int32_t  & outlet_diffusorFeedback,
int32_t  & outlet_diffusorDamping,
int32_t & outlet_predelayMsecs,
int32_t  & outlet_pitchFeedback,
int32_t  & outlet_pitchDamping,
int32_t  & outlet_pitchRatioLeft,
int32_t  & outlet_pitchMix,
int32_t  & outlet_pitchRatioRight,
int param_program,
int param_save  ){
static int savePressed = false;

static int lastProg = -1;

if (param_program != lastProg)
{
	lastProg = param_program;
	memcpy(( void*)&dataSet, (const void*)&reverbPrograms[lastProg %8], sizeof(dataSet));
}


if (param_save  && !savePressed)
{
	savePressed = true;
	printDataSet();
}
else
if (!param_save  && savePressed)
{
	savePressed = false;
}

outlet_modulationTime = dataSet.modulationTime;
outlet_modulationFeedback = dataSet.modulationFeedback;
outlet_modulationDamp = dataSet.modulationDamp;
outlet_modulationDepth = dataSet.modulationDepth;
outlet_modulationTime2 = dataSet.modulationTime2;
outlet_modulationDepth2 = dataSet.modulationDepth2;
outlet_inputDiffusorFeedback = dataSet.inputDiffusorFeedback;
outlet_inputDiffusorDamping = dataSet.inputDiffusorDamping;
outlet_tankFeedback = dataSet.tankFeedback;
outlet_diffusorFeedback = dataSet.diffusorFeedback;
outlet_diffusorDamping = dataSet.diffusorDamping;
outlet_predelayMsecs = dataSet.predelayMsecs;
outlet_pitchFeedback = dataSet.pitchFeedback;
outlet_pitchDamping = dataSet.pitchDamping;
outlet_pitchRatioLeft = dataSet.pitchRatioLeft;
outlet_pitchMix = dataSet.pitchMix;
outlet_pitchRatioRight = dataSet.pitchRatioRight;
int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {

}
}
void midiInHandler(rootc*parent, midi_message_t midi_message) {
uint8_t dev = 0;
uint8_t port = midiMessageGetPort(midi_message);
uint8_t status = midiMessageGetB0(midi_message);
uint8_t data1 = midiMessageGetB1(midi_message);
uint8_t data2 = midiMessageGetB2(midi_message);
{
LogTextMessage("%02x %d %d %f", status, data1, data2, q27_to_float(data2<<20));
if (status==0xB0)
{
	switch(data1)
	{
		case 16:
			dataSet.inputDiffusorFeedback = data2<<20;
			break;
		case 17:
			dataSet.inputDiffusorDamping = data2<<20;
			break;
		case 18:
			dataSet.predelayMsecs = data2<<1;
			break;

		case 20:
			dataSet.modulationFeedback = data2<<20;
			break;
		case 21:
			dataSet.modulationDamp = data2<<20;
			break;
		case 22:
			dataSet.modulationTime = data2<<4;
			break;
		case 23:
			dataSet.modulationDepth = data2<<1;
			break;
		case 24:
			dataSet.modulationTime2 = data2<<4;
			break;
		case 25:
			dataSet.modulationDepth2 = data2<<1;
			break;
		case 28:
			dataSet.diffusorFeedback = data2<<20;
			break;
		case 29:
			dataSet.diffusorDamping = data2<<20;
			break;
		case 46:
			dataSet.pitchFeedback = data2<<20;
			break;
		case 47:
			dataSet.pitchDamping = data2<<20;
			break;
		case 48:
			dataSet.pitchRatioLeft = float_to_q27(mapRatio[data2]);
			break;
		case 49:
			dataSet.pitchMix = data2<<20;
			break;
		case 52:
			dataSet.pitchRatioRight = float_to_q27(mapRatio[data2]);
			break;
		case 58:
			dataSet.tankFeedback = data2<<20;
			break;
	}
}
}
}
}
;class instancedownmix8__1{
  public: // v1
  rootc *parent;

  public: int init(rootc * _parent) {
parent = _parent;


 return 0;
}
  public: void dispose() {


}
  public: void dsp (rootc * parent,
const int32buffer  inlet_p__in1,
const int32buffer  inlet_p__in2,
const int32buffer  inlet_n__in3,
const int32buffer  inlet_p__in4,
const int32buffer  inlet_n__in5,
const int32buffer  inlet_n__in6,
const int32buffer  inlet_n__in7,
const int32buffer  inlet_p__in8,
int32buffer  & outlet_out  ){

int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
int32_t a = inlet_p__in1[buffer_index];

a += inlet_p__in2[buffer_index];
a -= inlet_n__in3[buffer_index];
a += inlet_p__in4[buffer_index];
a -= inlet_n__in5[buffer_index];
a -= inlet_n__in6[buffer_index];
a -= inlet_n__in7[buffer_index];
a += inlet_p__in8[buffer_index];
outlet_out[buffer_index] = a;
}
}
}
;class instancemodFdbkDelay__2{
  public: // v1
  rootc *parent;
int32_t *d;
int32_t head=0;
int32_t tail=2;
int32_t currentWidth;
int32_t currentPhase=0;
int32_t krateStep=0;
int32_t damper;
int32_t prev;
int32_t offset;
  public: int init(rootc * _parent) {
parent = _parent;
static int32_t sram_buffer1[1331];
d = sram_buffer1;
head = 0;

 return 0;
}
  public: void dispose() {


}
  public: void dsp (rootc * parent,
const int32buffer  inlet_in,
const int32_t  inlet_fdbk,
const int32_t  inlet_damper,
const int32_t inlet_modulationTime,
const int32_t inlet_moddepth,
int32buffer  & outlet_out  ){
krateStep++;

if (krateStep < inlet_modulationTime)
{
	currentPhase=inlet_moddepth*krateStep/inlet_modulationTime;
}
else if (krateStep < inlet_modulationTime*2)
{
	currentPhase=inlet_moddepth*(inlet_modulationTime*2-krateStep)/inlet_modulationTime;
}
else
{
	krateStep = 0;
}

damper = inlet_damper;

int32_t fdbk = inlet_fdbk<<3; 

tail = head+2+currentPhase;

if (tail>= 1331)
	tail-=1331;
int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
/*
  out =  delay+in*factor
  delayIn = delay*factor + in
 */

outlet_out[buffer_index] = ___SMMLA(-inlet_in[buffer_index]<<2, fdbk, d[tail]);
int32_t valueD = ___SMMLA(d[tail]<<2, fdbk, inlet_in[buffer_index]);

tail++;
if (tail >= 1331)
{
	tail = 0;
}

int32_t delta = prev-valueD;
offset =___SMMUL(damper<<3,offset<<2)+delta;
prev = valueD;

d[head++] = offset+prev;

if (head >= 1331)
{
	head = 0;
}
}
}
}
;class instancedelay3taps__2{
  public: // v1
  rootc *parent;
const int32_t maxSize = 8192;
int32_t *d;
int32_t head;
int32_t tap1, tap2, tap3;
  public: int init(rootc * _parent) {
parent = _parent;
d=(int32_t *) sdram_malloc(sizeof(int32_t)*maxSize);
memset(d, 0, sizeof(int32_t)*maxSize);
head = 0;
tap1 = 0;
tap2 = 0;
tap3 = 0;

 return 0;
}
  public: void dispose() {
ax_free(d);

}
  public: void dsp (rootc * parent,
const int32buffer  inlet_in,
int32buffer  & outlet_out,
int32buffer  & outlet_tap1,
int32buffer  & outlet_tap2,
int32buffer  & outlet_tap3,
int param_width,
int param_tap1,
int param_tap2,
int param_tap3  ){

int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
outlet_out[buffer_index] = d[head];
d[head++] = inlet_in[buffer_index];

if (head >= param_width)
{
	head = 0;
	tap1 = param_tap1;
	tap2 = param_tap2;
	tap3 = param_tap3;
}

outlet_tap1[buffer_index] = d[tap1];
tap1++;
if (tap1 >= param_width)
	tap1=0;

outlet_tap2[buffer_index] = d[tap2];
tap2++;
if (tap2 >= param_width)
	tap2=0;

outlet_tap3[buffer_index] = d[tap3];
tap3++;
if (tap3 >= param_width)
	tap3=0;
}
}
}
;class instancediffusor__damper__tap2__3{
  public: // v1
  rootc *parent;
class FeedbackRing
{
public:
	FeedbackRing()
	: m_size(0)
	, m_head(0)
	, m_gain(0)
	, prev(0)
	, offset(0)
	{
	}

	~FeedbackRing()
	{
		if (m_buffer)
			ax_free(m_buffer);	
	}

	void dispose()
	{
		if (m_buffer)
			ax_free(m_buffer);	
		m_buffer = 0;
	}

	void init(int size)
	{
		m_buffer = (int32_t *) sdram_malloc(size*sizeof(int32_t));
		LogTextMessage("diffuser mem: %d", size);
		m_size = size;
		memset(m_buffer, 0, sizeof(int32_t)*m_size);
		m_head = 0;
		m_adjustGain = false;
		prev = 0;
		offset = 0;
	}

	void gain(int32_t gain_q27, bool adjustGain=false)
	{
		if (float_to_q27(0.97) <= gain_q27)
			gain_q27 = float_to_q27(0.97);
		m_gain = gain_q27<<3;
		m_outgain = 8<<27;
		if (adjustGain)
		{
			adjustOutputGain();
		}
	}

	void setDamping(int32_t damping)
	{
		m_damper = damping;
	}

	void adjustOutputGain()
	{
		const float cx=1.0, cy=0.35, cd=0.73;
		float currentAdjust = 1.0/((1.0f / (cx-fabs(q27_to_float(m_gain>>3)))) * cd + cy);
		m_outgain = float_to_q27(currentAdjust)<<3;
		m_adjustGain = true;

	}

	
	int32_t tap(int32_t rel_position)
	{
		int h=m_head-rel_position+m_size;
		if (h >= m_size)
			h-=m_size;
		return m_buffer[h];
	}

	int32_t update(const int32buffer in, int32buffer &out, int32_t tap1Pos, int32buffer &tap1, int32_t tap2Pos,int32buffer &tap2)
	{
		
		int head = m_head;
		for (int i=0; i < BUFSIZE; ++i)
		{
			int32_t ret = ___SMMLA(in[i]<<2, m_gain, m_buffer[m_head]);
			int32_t satValue = ___SMMLA(m_buffer[m_head]<<2, -m_gain, in[i]); // N.B. negativ feedback
			int32_t delta = prev-satValue;
			offset =___SMMUL(m_damper<<3,offset<<2)+delta;
			prev = satValue;
		
			m_buffer[m_head++] = offset+satValue;
			if (m_head >= m_size)
				m_head = 0;
			if (m_adjustGain)
			{
				ret = ___SMMUL(ret, m_outgain)<<2;
			}
			out[i] = ret;
			tap1[i] = tap(tap1Pos);
			tap2[i] = tap(tap2Pos);
		}
	}

private:
	int32_t m_size;
	int32_t m_head;
	int32_t m_gain;
	int32_t m_outgain;
	bool m_adjustGain;
	int32_t *m_buffer;
     int32_t m_damper;
	int32_t prev;
	int32_t offset;
};

FeedbackRing rng;
  public: int init(rootc * _parent) {
parent = _parent;
rng.init(4253);

 return 0;
}
  public: void dispose() {
rng.dispose();

}
  public: void dsp (rootc * parent,
const int32buffer  inlet_in,
const int32_t  inlet_fdbk,
const int32_t  inlet_damping,
int32buffer  & outlet_out,
int32buffer  & outlet_tap1,
int32buffer  & outlet_tap2,
int param_tap1,
int param_tap2  ){
static int32_t oldFdbk=123;

if (oldFdbk != inlet_fdbk)
{
	oldFdbk = inlet_fdbk;
	rng.gain(oldFdbk, true);
}

rng.setDamping(inlet_damping);

rng.update(inlet_in, outlet_out, param_tap1, outlet_tap1, param_tap2, outlet_tap2);
int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {

}
}
}
;class instancedelay2taps__2{
  public: // v1
  rootc *parent;
const int32_t maxSize = 8192;
int32_t *d;
int32_t head;
int32_t tap1, tap2, tap3;
  public: int init(rootc * _parent) {
parent = _parent;
d=(int32_t *) sdram_malloc(sizeof(int32_t)*maxSize);
memset(d,0, sizeof(int32_t)*maxSize);

head = 0;
tap1 = 0;
tap2 = 0;
tap3 = 0;

 return 0;
}
  public: void dispose() {


}
  public: void dsp (rootc * parent,
const int32buffer  inlet_in,
int32buffer  & outlet_out,
int32buffer  & outlet_tap1,
int32buffer  & outlet_tap2,
int32buffer  & outlet_tap3,
int param_width,
int param_tap1,
int param_tap2,
int param_tap3  ){

int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
outlet_out[buffer_index] = d[head];
d[head++] = inlet_in[buffer_index];

if (head >= param_width)
{
	head = 0;
	tap1 = param_tap1;
	tap2 = param_tap2;
	tap3 = param_tap3;
}

outlet_tap1[buffer_index] = d[tap1];
tap1++;
if (tap1 >= param_width)
	tap1 = 0;

outlet_tap2[buffer_index] = d[tap2];
tap2++;
if (tap2 >= param_width)
	tap2 = 0;

outlet_tap3[buffer_index] = d[tap3];
tap3++;
if (tap3 >= param_width)
	tap3 = 0;
}
}
}
;/* object instances */
     instanceout__1 instanceout__1_i;
     instanceoutconfig__1 instanceoutconfig__1_i;
     instanceinconfig__1 instanceinconfig__1_i;
     instancedryWetMixStereo__1 instancedryWetMixStereo__1_i;
     instanceDistortion__1 instanceDistortion__1_i;
     instanceDeluxeVerbResponse__1 instanceDeluxeVerbResponse__1_i;
     instancepredelay__slap instancepredelay__slap_i;
     instanceinput__diffusor__2 instanceinput__diffusor__2_i;
     instancein__1 instancein__1_i;
     instancelong__time__decay__feedback instancelong__time__decay__feedback_i;
     instanceharpReso__1 instanceharpReso__1_i;
     instancestereo__highpass instancestereo__highpass_i;
     instancediffusor__damper__tap2__2 instancediffusor__damper__tap2__2_i;
     instancedelay2taps__1 instancedelay2taps__1_i;
     instancemodFdbkDelay__1 instancemodFdbkDelay__1_i;
     instancedelay3taps__1 instancedelay3taps__1_i;
     instancedownmix instancedownmix_i;
     instancepitchDelay__1 instancepitchDelay__1_i;
     instancei__2 instancei__2_i;
     instanceAkaiMidiMixReverbPlate__1 instanceAkaiMidiMixReverbPlate__1_i;
     instancedownmix8__1 instancedownmix8__1_i;
     instancemodFdbkDelay__2 instancemodFdbkDelay__2_i;
     instancedelay3taps__2 instancedelay3taps__2_i;
     instancediffusor__damper__tap2__3 instancediffusor__damper__tap2__3_i;
     instancedelay2taps__2 instancedelay2taps__2_i;
/* net latches */
    int32buffer  net0Latch;
    int32buffer  net10Latch;
    int32buffer  net11Latch;
    int32_t  net12Latch;
    int32buffer  net16Latch;
    int32buffer  net17Latch;
    int32buffer  net18Latch;
    int32buffer  net19Latch;
    int32buffer  net22Latch;
    int32buffer  net23Latch;
    int32buffer  net24Latch;
    int32buffer  net26Latch;
    int32buffer  net27Latch;
    int32_t  net29Latch;
    int32_t net30Latch;
    int32_t  net31Latch;
    int32_t net32Latch;
    int32_t net33Latch;
    int32_t  net34Latch;
    int32_t  net35Latch;
    int32buffer  net37Latch;
    int32buffer  net38Latch;
    int32_t  net40Latch;
    int32_t  net41Latch;
    int32buffer  net45Latch;
    int32_t  net47Latch;
    int32_t  net48Latch;
    int32_t  net49Latch;
    int32_t  net50Latch;
    int32_t  net51Latch;
    int32buffer  net52Latch;
    int32buffer  net53Latch;
    int32buffer  net54Latch;
    int32buffer  net55Latch;
static const int polyIndex = 0;
static const int n_ui_objects = 15;
ui_object_t ui_objects[n_ui_objects] = {
{ name : {'o','u','t','_','1', 0 , 0 , 0 ,}, nparams : 0, params : 0, param_names : 0, ndisplays : 2, displays : &display_metas[0], nobjects : 0, objects : 0},
{ name : {'d','r','y','W','e','t','M','i',}, nparams : 1, params : &PExch[0], param_names : &param_names[0], ndisplays : 0, displays : 0, nobjects : 0, objects : 0},
{ name : {'D','i','s','t','o','r','t','i',}, nparams : 13, params : &PExch[1], param_names : &param_names[1], ndisplays : 0, displays : 0, nobjects : 0, objects : 0},
{ name : {'D','e','l','u','x','e','V','e',}, nparams : 5, params : &PExch[14], param_names : &param_names[14], ndisplays : 0, displays : 0, nobjects : 0, objects : 0},
{ name : {'i','n','_','1', 0 , 0 , 0 , 0 ,}, nparams : 0, params : 0, param_names : 0, ndisplays : 2, displays : &display_metas[2], nobjects : 0, objects : 0},
{ name : {'s','t','e','r','e','o','_','h',}, nparams : 2, params : &PExch[19], param_names : &param_names[19], ndisplays : 0, displays : 0, nobjects : 0, objects : 0},
{ name : {'d','i','f','f','u','s','o','r',}, nparams : 2, params : &PExch[21], param_names : &param_names[21], ndisplays : 0, displays : 0, nobjects : 0, objects : 0},
{ name : {'d','e','l','a','y','2','t','a',}, nparams : 4, params : &PExch[23], param_names : &param_names[23], ndisplays : 0, displays : 0, nobjects : 0, objects : 0},
{ name : {'d','e','l','a','y','3','t','a',}, nparams : 4, params : &PExch[27], param_names : &param_names[27], ndisplays : 0, displays : 0, nobjects : 0, objects : 0},
{ name : {'p','i','t','c','h','D','e','l',}, nparams : 3, params : &PExch[31], param_names : &param_names[31], ndisplays : 0, displays : 0, nobjects : 0, objects : 0},
{ name : {'i','_','2', 0 , 0 , 0 , 0 , 0 ,}, nparams : 0, params : 0, param_names : 0, ndisplays : 1, displays : &display_metas[4], nobjects : 0, objects : 0},
{ name : {'A','k','a','i','M','i','d','i',}, nparams : 2, params : &PExch[34], param_names : &param_names[34], ndisplays : 0, displays : 0, nobjects : 0, objects : 0},
{ name : {'d','e','l','a','y','3','t','a',}, nparams : 4, params : &PExch[36], param_names : &param_names[36], ndisplays : 0, displays : 0, nobjects : 0, objects : 0},
{ name : {'d','i','f','f','u','s','o','r',}, nparams : 2, params : &PExch[40], param_names : &param_names[40], ndisplays : 0, displays : 0, nobjects : 0, objects : 0},
{ name : {'d','e','l','a','y','2','t','a',}, nparams : 4, params : &PExch[42], param_names : &param_names[42], ndisplays : 0, displays : 0, nobjects : 0, objects : 0},
};
   static int32_t * GetInitParams(void){
      static const int32_t p[46]= {
      10485760,
      124780544,
      4920,
      176,
      594,
      0,
      0,
      106954752,
      134217728,
      -4194304,
      51380224,
      68157440,
      0,
      47185920,
      1,
      134217728,
      8388608,
      20971520,
      29360128,
      -52009369,
      36700160,
      299,
      1965,
      5952,
      1726,
      4306,
      669,
      7182,
      3210,
      5850,
      569,
      4,
      134217728,
      0,
      3,
      0,
      6747,
      426,
      4758,
      3378,
      3061,
      536,
      5061,
      3194,
      194,
      1611
      };
      return (int32_t *)&p[0];
   }
      int32_t presets[NPRESETS][NPRESET_ENTRIES][2] = {
         {
           {1,0},
           {5,0},
           {6,0},
           {7,131072000},
           {8,134217728},
           {9,0},
           {10,71303168},
           {11,134217728},
           {12,-44040192},
           {13,31457280},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0}
         },
         {
           {1,134217728},
           {5,0},
           {6,0},
           {7,131072000},
           {8,134217728},
           {9,10485760},
           {10,108003328},
           {11,134217728},
           {12,0},
           {13,31457280},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0}
         },
         {
           {1,134217728},
           {5,0},
           {6,0},
           {7,131072000},
           {8,134217728},
           {9,8388608},
           {10,61865984},
           {11,134217728},
           {12,-37748736},
           {13,31457280},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0}
         },
         {
           {1,0},
           {5,0},
           {6,0},
           {7,131072000},
           {8,85983232},
           {9,8388608},
           {10,61865984},
           {11,134217728},
           {12,-37748736},
           {13,31457280},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0}
         },
         {
           {1,0},
           {5,0},
           {6,0},
           {7,131072000},
           {8,85983232},
           {9,8388608},
           {10,61865984},
           {11,134217728},
           {12,-37748736},
           {13,31457280},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0}
         },
         {
           {1,0},
           {5,0},
           {6,0},
           {7,131072000},
           {8,85983232},
           {9,8388608},
           {10,61865984},
           {11,134217728},
           {12,-37748736},
           {13,31457280},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0}
         },
         {
           {1,134217728},
           {5,0},
           {6,0},
           {7,131072000},
           {8,85983232},
           {9,8388608},
           {10,61865984},
           {11,134217728},
           {12,-37748736},
           {13,31457280},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0}
         },
         {
           {1,134217728},
           {5,0},
           {6,0},
           {7,131072000},
           {8,85983232},
           {9,8388608},
           {10,61865984},
           {11,134217728},
           {12,-37748736},
           {13,31457280},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0},
           {-1,0}
         }
      };
const int32_t * GetPresets(void){
  return (int32_t *)&presets[0][0][0];
}

void ApplyPreset(int index){
   if (!index) {
     int i;
     int32_t *p = GetInitParams();
     for(i=0;i<nparams;i++){
        parameter_setVal(&PExch[i], p[i], 0xFFEF);
     }
   }
   index--;
   if (index < NPRESETS) {
     PresetParamChange_t *pa = (PresetParamChange_t *)(GetPresets());
     PresetParamChange_t *p = &pa[index*NPRESET_ENTRIES];
       int i;
       for(i=0;i<NPRESET_ENTRIES;i++){
         PresetParamChange_t *pp = &p[i];
         if ((pp->pexIndex>=0)&&(pp->pexIndex<nparams)) {
            parameter_setVal(&PExch[pp->pexIndex],pp->value,0xFFEF);         }
         else break;
       }
   }
}
   static PExModulationTarget_t * GetModulationTable(void){
    static const PExModulationTarget_t PExModulationSources[NMODULATIONSOURCES][NMODULATIONTARGETS] = 
{{{-1,0}},
{{-1,0}},
{{-1,0}},
{{-1,0}},
{{-1,0}},
{{-1,0}},
{{-1,0}},
{{-1,0}}};
   return (PExModulationTarget_t *)&PExModulationSources[0][0];
   };
/* init */
int init() {
  int r = 0;
  r =  instanceout__1_i.init(this, displayVector[0], displayVector[1] );
  if (r) return r;
  r =  instanceoutconfig__1_i.init(this );
  if (r) return r;
  r =  instanceinconfig__1_i.init(this );
  if (r) return r;
  r =  instancedryWetMixStereo__1_i.init(this );
  if (r) return r;
  r =  instanceDistortion__1_i.init(this );
  if (r) return r;
  r =  instanceDeluxeVerbResponse__1_i.init(this );
  if (r) return r;
  r =  instancepredelay__slap_i.init(this );
  if (r) return r;
  r =  instanceinput__diffusor__2_i.init(this );
  if (r) return r;
  r =  instancein__1_i.init(this, displayVector[2], displayVector[3] );
  if (r) return r;
  r =  instancelong__time__decay__feedback_i.init(this );
  if (r) return r;
  r =  instanceharpReso__1_i.init(this );
  if (r) return r;
  r =  instancestereo__highpass_i.init(this );
  if (r) return r;
  r =  instancediffusor__damper__tap2__2_i.init(this );
  if (r) return r;
  r =  instancedelay2taps__1_i.init(this );
  if (r) return r;
  r =  instancemodFdbkDelay__1_i.init(this );
  if (r) return r;
  r =  instancedelay3taps__1_i.init(this );
  if (r) return r;
  r =  instancedownmix_i.init(this );
  if (r) return r;
  r =  instancepitchDelay__1_i.init(this );
  if (r) return r;
  r =  instancei__2_i.init(this, displayVector[4] );
  if (r) return r;
  r =  instanceAkaiMidiMixReverbPlate__1_i.init(this );
  if (r) return r;
  r =  instancedownmix8__1_i.init(this );
  if (r) return r;
  r =  instancemodFdbkDelay__2_i.init(this );
  if (r) return r;
  r =  instancedelay3taps__2_i.init(this );
  if (r) return r;
  r =  instancediffusor__damper__tap2__3_i.init(this );
  if (r) return r;
  r =  instancedelay2taps__2_i.init(this );
  if (r) return r;
// GenerateParamInitCodePlusPlusSub
   int i;
   int j;
   const int32_t *p;
   p = GetInitParams();
   for(j=0;j<46;j++){
      parameter_setVal(&PExch[j], p[j], 0xFFEE);
   }
   int32_t *pp = &PExModulationPrevVal[0][0];
   for(j=0;j<1*NMODULATIONSOURCES;j++){
      *pp = 0; pp++;
   }
  return 0;
}

/* dispose */
void dispose() {
   instancedelay2taps__2_i.dispose();
   instancediffusor__damper__tap2__3_i.dispose();
   instancedelay3taps__2_i.dispose();
   instancemodFdbkDelay__2_i.dispose();
   instancedownmix8__1_i.dispose();
   instanceAkaiMidiMixReverbPlate__1_i.dispose();
   instancei__2_i.dispose();
   instancepitchDelay__1_i.dispose();
   instancedownmix_i.dispose();
   instancedelay3taps__1_i.dispose();
   instancemodFdbkDelay__1_i.dispose();
   instancedelay2taps__1_i.dispose();
   instancediffusor__damper__tap2__2_i.dispose();
   instancestereo__highpass_i.dispose();
   instanceharpReso__1_i.dispose();
   instancelong__time__decay__feedback_i.dispose();
   instancein__1_i.dispose();
   instanceinput__diffusor__2_i.dispose();
   instancepredelay__slap_i.dispose();
   instanceDeluxeVerbResponse__1_i.dispose();
   instanceDistortion__1_i.dispose();
   instancedryWetMixStereo__1_i.dispose();
   instanceinconfig__1_i.dispose();
   instanceoutconfig__1_i.dispose();
   instanceout__1_i.dispose();
  this->~rootc();
}

/* krate */
void dsp (void) {
  int i;
//--------- <nets> -----------//
    int32buffer  net0;
    int32buffer  net1;
    int32buffer  net2;
    int32buffer  net3;
    int32buffer  net4;
    int32buffer  net5;
    int32buffer  net6;
    int32buffer  net7;
    int32buffer  net8;
    int32buffer  net9;
    int32buffer  net10;
    int32buffer  net11;
    int32_t  net12;
    int32buffer  net13;
    int32buffer  net14;
    int32buffer  net15;
    int32buffer  net16;
    int32buffer  net17;
    int32buffer  net18;
    int32buffer  net19;
    int32buffer  net20;
    int32buffer  net21;
    int32buffer  net22;
    int32buffer  net23;
    int32buffer  net24;
    int32buffer  net25;
    int32buffer  net26;
    int32buffer  net27;
    int32buffer  net28;
    int32_t  net29;
    int32_t net30;
    int32_t  net31;
    int32_t net32;
    int32_t net33;
    int32_t  net34;
    int32_t  net35;
    int32buffer  net36;
    int32buffer  net37;
    int32buffer  net38;
    int32buffer  net39;
    int32_t  net40;
    int32_t  net41;
    int32buffer  net42;
    int32_t net43;
    int32_t net44;
    int32buffer  net45;
    int32buffer  net46;
    int32_t  net47;
    int32_t  net48;
    int32_t  net49;
    int32_t  net50;
    int32_t  net51;
    int32buffer  net52;
    int32buffer  net53;
    int32buffer  net54;
    int32buffer  net55;
//--------- </nets> ----------//
//--------- <zero> ----------//
  int32_t UNCONNECTED_OUTPUT;
  static const int32_t UNCONNECTED_INPUT=0;
  static const int32buffer zerobuffer = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int32buffer UNCONNECTED_OUTPUT_BUFFER;
//--------- </zero> ----------//
//--------- <object calls> ----------//
  instanceout__1_i.dsp( this , net52Latch, net53Latch, displayVector[0], displayVector[1]);
  instanceoutconfig__1_i.dsp( this );
  instanceinconfig__1_i.dsp( this );
  instancedryWetMixStereo__1_i.dsp( this , net37Latch, net37Latch, net54Latch, net55Latch, net52, net53, PExch[PARAM_INDEX_dryWetMixStereo__1_balance].d.frac.finalvalue);
  instanceDistortion__1_i.dsp( this , net0Latch, net36, PExch[PARAM_INDEX_Distortion__1_dryWet].d.frac.finalvalue, PExch[PARAM_INDEX_Distortion__1_tone].d.intt.finalvalue, PExch[PARAM_INDEX_Distortion__1_splitLow].d.intt.finalvalue, PExch[PARAM_INDEX_Distortion__1_splitHigh].d.intt.finalvalue, PExch[PARAM_INDEX_Distortion__1_gainLow].d.frac.finalvalue, PExch[PARAM_INDEX_Distortion__1_biasLow].d.frac.finalvalue, PExch[PARAM_INDEX_Distortion__1_levelLow].d.frac.finalvalue, PExch[PARAM_INDEX_Distortion__1_gainMid].d.frac.finalvalue, PExch[PARAM_INDEX_Distortion__1_biasMid].d.frac.finalvalue, PExch[PARAM_INDEX_Distortion__1_levelMid].d.frac.finalvalue, PExch[PARAM_INDEX_Distortion__1_gainHigh].d.frac.finalvalue, PExch[PARAM_INDEX_Distortion__1_biasHigh].d.frac.finalvalue, PExch[PARAM_INDEX_Distortion__1_levelHigh].d.frac.finalvalue);
  instanceDeluxeVerbResponse__1_i.dsp( this , net36, net37, PExch[PARAM_INDEX_DeluxeVerbResponse__1_onoff].d.bin.finalvalue, PExch[PARAM_INDEX_DeluxeVerbResponse__1_level].d.frac.finalvalue, PExch[PARAM_INDEX_DeluxeVerbResponse__1_gainLow].d.frac.finalvalue, PExch[PARAM_INDEX_DeluxeVerbResponse__1_gainMid].d.frac.finalvalue, PExch[PARAM_INDEX_DeluxeVerbResponse__1_gainHigh].d.frac.finalvalue);
  instancepredelay__slap_i.dsp( this , net37, net32Latch, net5);
  instanceinput__diffusor__2_i.dsp( this , net5, net40Latch, net41Latch, net42);
  instancein__1_i.dsp( this , net0, UNCONNECTED_OUTPUT_BUFFER, displayVector[2], displayVector[3]);
  instancelong__time__decay__feedback_i.dsp( this , net42, net22Latch, net19Latch, net31Latch, net1, net2);
  instanceharpReso__1_i.dsp( this , zerobuffer, 0 , 0 , UNCONNECTED_OUTPUT_BUFFER);
  instancestereo__highpass_i.dsp( this , net1, net2, net3, net4, PExch[PARAM_INDEX_stereo__highpass_pitch].d.frac.finalvalue, PExch[PARAM_INDEX_stereo__highpass_reso].d.frac.finalvalue);
  instancediffusor__damper__tap2__2_i.dsp( this , net45Latch, net34Latch, net35Latch, net6, net7, net8, PExch[PARAM_INDEX_diffusor__damper__tap2__2_tap1].d.intt.finalvalue, PExch[PARAM_INDEX_diffusor__damper__tap2__2_tap2].d.intt.finalvalue);
  instancedelay2taps__1_i.dsp( this , net6, net19, net20, net21, net28, PExch[PARAM_INDEX_delay2taps__1_width].d.intt.finalvalue, PExch[PARAM_INDEX_delay2taps__1_tap1].d.intt.finalvalue, PExch[PARAM_INDEX_delay2taps__1_tap2].d.intt.finalvalue, PExch[PARAM_INDEX_delay2taps__1_tap3].d.intt.finalvalue);
  instancemodFdbkDelay__1_i.dsp( this , net3, net29Latch, net12Latch, net33Latch, net30Latch, net25);
  instancedelay3taps__1_i.dsp( this , net38Latch, net45, net13, net14, net15, PExch[PARAM_INDEX_delay3taps__1_width].d.intt.finalvalue, PExch[PARAM_INDEX_delay3taps__1_tap1].d.intt.finalvalue, PExch[PARAM_INDEX_delay3taps__1_tap2].d.intt.finalvalue, PExch[PARAM_INDEX_delay3taps__1_tap3].d.intt.finalvalue);
  instancedownmix_i.dsp( this , net13, net14, net10Latch, net23Latch, net18Latch, net7, net20, net28, net54);
  instancepitchDelay__1_i.dsp( this , net25, net26Latch, net51Latch, net50Latch, net48Latch, net49Latch, net47Latch, net38, net39, PExch[PARAM_INDEX_pitchDelay__1_fadeSize].d.intt.finalvalue, PExch[PARAM_INDEX_pitchDelay__1_p1].d.frac.finalvalue, PExch[PARAM_INDEX_pitchDelay__1_fdbk].d.frac.finalvalue);
  instancei__2_i.dsp( this , 0, displayVector[4]);
  instanceAkaiMidiMixReverbPlate__1_i.dsp( this , net29, net12, net33, net43, net30, net44, net40, net41, net31, net34, net35, net32, net51, net50, net48, net47, net49, PExch[PARAM_INDEX_AkaiMidiMixReverbPlate__1_program].d.intt.finalvalue, PExch[PARAM_INDEX_AkaiMidiMixReverbPlate__1_save].d.bin.finalvalue);
  instancedownmix8__1_i.dsp( this , net16Latch, net17Latch, net8, net21, net15, net11Latch, net24Latch, net27Latch, net55);
  instancemodFdbkDelay__2_i.dsp( this , net4, net29, net12, net43, net44, net26);
  instancedelay3taps__2_i.dsp( this , net39, net46, net16, net17, net18, PExch[PARAM_INDEX_delay3taps__2_width].d.intt.finalvalue, PExch[PARAM_INDEX_delay3taps__2_tap1].d.intt.finalvalue, PExch[PARAM_INDEX_delay3taps__2_tap2].d.intt.finalvalue, PExch[PARAM_INDEX_delay3taps__2_tap3].d.intt.finalvalue);
  instancediffusor__damper__tap2__3_i.dsp( this , net46, net34, net35, net9, net10, net11, PExch[PARAM_INDEX_diffusor__damper__tap2__3_tap1].d.intt.finalvalue, PExch[PARAM_INDEX_diffusor__damper__tap2__3_tap2].d.intt.finalvalue);
  instancedelay2taps__2_i.dsp( this , net9, net22, net23, net24, net27, PExch[PARAM_INDEX_delay2taps__2_width].d.intt.finalvalue, PExch[PARAM_INDEX_delay2taps__2_tap1].d.intt.finalvalue, PExch[PARAM_INDEX_delay2taps__2_tap2].d.intt.finalvalue, PExch[PARAM_INDEX_delay2taps__2_tap3].d.intt.finalvalue);
//--------- </object calls> ----------//
//--------- <net latch copy> ----------//
   for(i=0;i<BUFSIZE;i++)
      net0Latch[i]=net0[i];
   for(i=0;i<BUFSIZE;i++)
      net10Latch[i]=net10[i];
   for(i=0;i<BUFSIZE;i++)
      net11Latch[i]=net11[i];
net12Latch = net12;
   for(i=0;i<BUFSIZE;i++)
      net16Latch[i]=net16[i];
   for(i=0;i<BUFSIZE;i++)
      net17Latch[i]=net17[i];
   for(i=0;i<BUFSIZE;i++)
      net18Latch[i]=net18[i];
   for(i=0;i<BUFSIZE;i++)
      net19Latch[i]=net19[i];
   for(i=0;i<BUFSIZE;i++)
      net22Latch[i]=net22[i];
   for(i=0;i<BUFSIZE;i++)
      net23Latch[i]=net23[i];
   for(i=0;i<BUFSIZE;i++)
      net24Latch[i]=net24[i];
   for(i=0;i<BUFSIZE;i++)
      net26Latch[i]=net26[i];
   for(i=0;i<BUFSIZE;i++)
      net27Latch[i]=net27[i];
net29Latch = net29;
net30Latch = net30;
net31Latch = net31;
net32Latch = net32;
net33Latch = net33;
net34Latch = net34;
net35Latch = net35;
   for(i=0;i<BUFSIZE;i++)
      net37Latch[i]=net37[i];
   for(i=0;i<BUFSIZE;i++)
      net38Latch[i]=net38[i];
net40Latch = net40;
net41Latch = net41;
   for(i=0;i<BUFSIZE;i++)
      net45Latch[i]=net45[i];
net47Latch = net47;
net48Latch = net48;
net49Latch = net49;
net50Latch = net50;
net51Latch = net51;
   for(i=0;i<BUFSIZE;i++)
      net52Latch[i]=net52[i];
   for(i=0;i<BUFSIZE;i++)
      net53Latch[i]=net53[i];
   for(i=0;i<BUFSIZE;i++)
      net54Latch[i]=net54[i];
   for(i=0;i<BUFSIZE;i++)
      net55Latch[i]=net55[i];
//--------- </net latch copy> ----------//
}

void midiInHandler(rootc *parent, midi_message_t midi_message){
instanceAkaiMidiMixReverbPlate__1_i.midiInHandler(this, midi_message);
}

};

extern "C" {
  int getInstanceSize() {
    return sizeof(rootc);
  }

  int initInstance(PatchInstance *instance /*,... args */) {
    // placement new
    rootc * _inst = new((void *)instance) rootc();
    return _inst->init();
  }
}
