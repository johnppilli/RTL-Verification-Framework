// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Varbiter.h for the primary calling header

#ifndef VERILATED_VARBITER___024ROOT_H_
#define VERILATED_VARBITER___024ROOT_H_  // guard

#include "verilated.h"


class Varbiter__Syms;

class alignas(VL_CACHE_LINE_BYTES) Varbiter___024root final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    VL_IN8(clk,0,0);
    VL_IN8(rst_n,0,0);
    VL_IN8(req,3,0);
    VL_OUT8(grant,3,0);
    VL_OUT8(valid,0,0);
    CData/*1:0*/ arbiter__DOT__ptr;
    CData/*1:0*/ arbiter__DOT__winner;
    CData/*1:0*/ arbiter__DOT__unnamedblk1__DOT__unnamedblk2__DOT__idx;
    CData/*0:0*/ __Vsampled_TOP__rst_n;
    CData/*3:0*/ __Vsampled_TOP__grant;
    CData/*3:0*/ __Vsampled_TOP__req;
    CData/*0:0*/ __Vsampled_TOP__valid;
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
    Varbiter__Syms* const vlSymsp;

    // CONSTRUCTORS
    Varbiter___024root(Varbiter__Syms* symsp, const char* v__name);
    ~Varbiter___024root();
    VL_UNCOPYABLE(Varbiter___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
