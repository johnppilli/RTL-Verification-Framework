// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vfifo.h for the primary calling header

#include "Vfifo__pch.h"
#include "Vfifo__Syms.h"
#include "Vfifo___024root.h"

#ifdef VL_DEBUG
VL_ATTR_COLD void Vfifo___024root___dump_triggers__ico(Vfifo___024root* vlSelf);
#endif  // VL_DEBUG

void Vfifo___024root___eval_triggers__ico(Vfifo___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfifo___024root___eval_triggers__ico\n"); );
    Vfifo__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VicoTriggered.setBit(0U, (IData)(vlSelfRef.__VicoFirstIteration));
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vfifo___024root___dump_triggers__ico(vlSelf);
    }
#endif
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vfifo___024root___dump_triggers__act(Vfifo___024root* vlSelf);
#endif  // VL_DEBUG

void Vfifo___024root___eval_triggers__act(Vfifo___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfifo___024root___eval_triggers__act\n"); );
    Vfifo__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VactTriggered.setBit(0U, ((IData)(vlSelfRef.clk) 
                                          & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__clk__0))));
    vlSelfRef.__VactTriggered.setBit(1U, ((~ (IData)(vlSelfRef.rst_n)) 
                                          & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__rst_n__0)));
    vlSelfRef.__Vtrigprevexpr___TOP__clk__0 = vlSelfRef.clk;
    vlSelfRef.__Vtrigprevexpr___TOP__rst_n__0 = vlSelfRef.rst_n;
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vfifo___024root___dump_triggers__act(vlSelf);
    }
#endif
}

VL_INLINE_OPT void Vfifo___024root___nba_sequent__TOP__0(Vfifo___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfifo___024root___nba_sequent__TOP__0\n"); );
    Vfifo__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (vlSymsp->_vm_contextp__->assertOnGet(1, 1)) {
        if (VL_UNLIKELY(((1U & (~ ((~ (IData)(vlSelfRef.__Vsampled_TOP__rst_n)) 
                                   | (~ ((IData)(vlSelfRef.__Vsampled_TOP__full) 
                                         & (IData)(vlSelfRef.__Vsampled_TOP__empty))))))))) {
            VL_WRITEF_NX("[%0t] %%Error: fifo.sv:75: Assertion failed in %Nfifo: fifo reported full and empty at once\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name());
            VL_STOP_MT("rtl/fifo.sv", 75, "");
        }
    }
    if (vlSymsp->_vm_contextp__->assertOnGet(1, 1)) {
        if (VL_UNLIKELY(((1U & (~ ((~ (IData)(vlSelfRef.__Vsampled_TOP__rst_n)) 
                                   | (8U >= (IData)(vlSelfRef.__Vsampled_TOP__fifo__DOT__count_reg)))))))) {
            VL_WRITEF_NX("[%0t] %%Error: fifo.sv:72: Assertion failed in %Nfifo: fifo count exceeded depth\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name());
            VL_STOP_MT("rtl/fifo.sv", 72, "");
        }
    }
    if (vlSymsp->_vm_contextp__->assertOnGet(1, 1)) {
        if (VL_UNLIKELY(((1U & (~ ((~ (IData)(vlSelfRef.__Vsampled_TOP__rst_n)) 
                                   | ((~ (IData)(vlSelfRef.__Vsampled_TOP__full)) 
                                      | (8U == (IData)(vlSelfRef.__Vsampled_TOP__fifo__DOT__count_reg))))))))) {
            VL_WRITEF_NX("[%0t] %%Error: fifo.sv:78: Assertion failed in %Nfifo: fifo full flag inconsistent with count\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name());
            VL_STOP_MT("rtl/fifo.sv", 78, "");
        }
    }
    if (vlSymsp->_vm_contextp__->assertOnGet(1, 1)) {
        if (VL_UNLIKELY(((1U & (~ ((~ (IData)(vlSelfRef.__Vsampled_TOP__rst_n)) 
                                   | ((~ (IData)(vlSelfRef.__Vsampled_TOP__empty)) 
                                      | (0U == (IData)(vlSelfRef.__Vsampled_TOP__fifo__DOT__count_reg))))))))) {
            VL_WRITEF_NX("[%0t] %%Error: fifo.sv:81: Assertion failed in %Nfifo: fifo empty flag inconsistent with count\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name());
            VL_STOP_MT("rtl/fifo.sv", 81, "");
        }
    }
    if (vlSymsp->_vm_contextp__->assertOnGet(1, 1)) {
        if (VL_UNLIKELY(((1U & (~ ((~ (IData)(vlSelfRef.__Vsampled_TOP__rst_n)) 
                                   | ((~ (IData)(vlSelfRef.__Vsampled_TOP__fifo__DOT___Vpast_0_0)) 
                                      | ((IData)(vlSelfRef.__Vsampled_TOP__fifo__DOT__count_reg) 
                                         == ((IData)(1U) 
                                             + (IData)(vlSelfRef.__Vsampled_TOP__fifo__DOT___Vpast_1_0)))))))))) {
            VL_WRITEF_NX("[%0t] %%Error: fifo.sv:86: Assertion failed in %Nfifo: fifo count did not track a write\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name());
            VL_STOP_MT("rtl/fifo.sv", 86, "");
        }
    }
    vlSelfRef.fifo__DOT___Vpast_1_0 = vlSelfRef.__Vsampled_TOP__fifo__DOT__count_reg;
    vlSelfRef.fifo__DOT___Vpast_0_0 = ((IData)(vlSelfRef.__Vsampled_TOP__rst_n) 
                                       & ((IData)(vlSelfRef.__Vsampled_TOP__fifo__DOT__do_write) 
                                          & (~ (IData)(vlSelfRef.__Vsampled_TOP__fifo__DOT__do_read))));
}
