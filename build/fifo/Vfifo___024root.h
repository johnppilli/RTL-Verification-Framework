// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vfifo.h for the primary calling header

#ifndef VERILATED_VFIFO___024ROOT_H_
#define VERILATED_VFIFO___024ROOT_H_  // guard

#include "verilated.h"


class Vfifo__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vfifo___024root final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    VL_IN8(clk,0,0);
    VL_IN8(rst_n,0,0);
    VL_IN8(wr_en,0,0);
    VL_IN8(data_in,7,0);
    VL_IN8(rd_en,0,0);
    VL_OUT8(data_out,7,0);
    VL_OUT8(full,0,0);
    VL_OUT8(empty,0,0);
    VL_OUT8(count,3,0);
    CData/*2:0*/ fifo__DOT__wr_ptr;
    CData/*2:0*/ fifo__DOT__rd_ptr;
    CData/*3:0*/ fifo__DOT__count_reg;
    CData/*0:0*/ fifo__DOT__do_write;
    CData/*0:0*/ fifo__DOT__do_read;
    CData/*0:0*/ fifo__DOT___Vpast_0_0;
    CData/*3:0*/ fifo__DOT___Vpast_1_0;
    CData/*0:0*/ __Vsampled_TOP__rst_n;
    CData/*3:0*/ __Vsampled_TOP__fifo__DOT__count_reg;
    CData/*0:0*/ __Vsampled_TOP__full;
    CData/*0:0*/ __Vsampled_TOP__empty;
    CData/*0:0*/ __Vsampled_TOP__fifo__DOT___Vpast_0_0;
    CData/*3:0*/ __Vsampled_TOP__fifo__DOT___Vpast_1_0;
    CData/*0:0*/ __Vsampled_TOP__fifo__DOT__do_write;
    CData/*0:0*/ __Vsampled_TOP__fifo__DOT__do_read;
    CData/*0:0*/ __VstlFirstIteration;
    CData/*0:0*/ __VicoFirstIteration;
    CData/*0:0*/ __Vtrigprevexpr___TOP__clk__0;
    CData/*0:0*/ __Vtrigprevexpr___TOP__rst_n__0;
    CData/*0:0*/ __VactContinue;
    IData/*31:0*/ __VactIterCount;
    VlUnpacked<CData/*7:0*/, 8> fifo__DOT__memory;
    VlUnpacked<CData/*0:0*/, 2> __Vm_traceActivity;
    VlTriggerVec<1> __VstlTriggered;
    VlTriggerVec<1> __VicoTriggered;
    VlTriggerVec<2> __VactTriggered;
    VlTriggerVec<2> __VnbaTriggered;

    // INTERNAL VARIABLES
    Vfifo__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vfifo___024root(Vfifo__Syms* symsp, const char* v__name);
    ~Vfifo___024root();
    VL_UNCOPYABLE(Vfifo___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
