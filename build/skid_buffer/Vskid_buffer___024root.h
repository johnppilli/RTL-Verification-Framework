// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vskid_buffer.h for the primary calling header

#ifndef VERILATED_VSKID_BUFFER___024ROOT_H_
#define VERILATED_VSKID_BUFFER___024ROOT_H_  // guard

#include "verilated.h"


class Vskid_buffer__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vskid_buffer___024root final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    VL_IN8(clk,0,0);
    VL_IN8(rst_n,0,0);
    VL_IN8(s_valid,0,0);
    VL_OUT8(s_ready,0,0);
    VL_IN8(s_data,7,0);
    VL_OUT8(m_valid,0,0);
    VL_IN8(m_ready,0,0);
    VL_OUT8(m_data,7,0);
    CData/*0:0*/ skid_buffer__DOT__skid_valid;
    CData/*7:0*/ skid_buffer__DOT__skid_data;
    CData/*0:0*/ skid_buffer__DOT__out_valid;
    CData/*7:0*/ skid_buffer__DOT__out_data;
    CData/*0:0*/ skid_buffer__DOT__accept;
    CData/*0:0*/ skid_buffer__DOT___Vpast_0_0;
    CData/*7:0*/ skid_buffer__DOT___Vpast_1_0;
    CData/*0:0*/ __Vsampled_TOP__rst_n;
    CData/*0:0*/ __Vsampled_TOP__skid_buffer__DOT___Vpast_0_0;
    CData/*0:0*/ __Vsampled_TOP__m_valid;
    CData/*7:0*/ __Vsampled_TOP__skid_buffer__DOT___Vpast_1_0;
    CData/*7:0*/ __Vsampled_TOP__m_data;
    CData/*0:0*/ __Vsampled_TOP__skid_buffer__DOT__skid_valid;
    CData/*0:0*/ __Vsampled_TOP__skid_buffer__DOT__out_valid;
    CData/*0:0*/ __Vsampled_TOP__m_ready;
    CData/*0:0*/ __VstlFirstIteration;
    CData/*0:0*/ __VicoFirstIteration;
    CData/*0:0*/ __Vtrigprevexpr___TOP__clk__0;
    CData/*0:0*/ __Vtrigprevexpr___TOP__rst_n__0;
    CData/*0:0*/ __VactContinue;
    IData/*31:0*/ __VactIterCount;
    VlTriggerVec<1> __VstlTriggered;
    VlTriggerVec<1> __VicoTriggered;
    VlTriggerVec<2> __VactTriggered;
    VlTriggerVec<2> __VnbaTriggered;

    // INTERNAL VARIABLES
    Vskid_buffer__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vskid_buffer___024root(Vskid_buffer__Syms* symsp, const char* v__name);
    ~Vskid_buffer___024root();
    VL_UNCOPYABLE(Vskid_buffer___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
