// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Varbiter.h for the primary calling header

#include "Varbiter__pch.h"
#include "Varbiter__Syms.h"
#include "Varbiter___024root.h"

#ifdef VL_DEBUG
VL_ATTR_COLD void Varbiter___024root___dump_triggers__ico(Varbiter___024root* vlSelf);
#endif  // VL_DEBUG

void Varbiter___024root___eval_triggers__ico(Varbiter___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Varbiter___024root___eval_triggers__ico\n"); );
    Varbiter__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VicoTriggered.setBit(0U, (IData)(vlSelfRef.__VicoFirstIteration));
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Varbiter___024root___dump_triggers__ico(vlSelf);
    }
#endif
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Varbiter___024root___dump_triggers__act(Varbiter___024root* vlSelf);
#endif  // VL_DEBUG

void Varbiter___024root___eval_triggers__act(Varbiter___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Varbiter___024root___eval_triggers__act\n"); );
    Varbiter__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
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
        Varbiter___024root___dump_triggers__act(vlSelf);
    }
#endif
}

VL_INLINE_OPT void Varbiter___024root___nba_sequent__TOP__0(Varbiter___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Varbiter___024root___nba_sequent__TOP__0\n"); );
    Varbiter__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (vlSymsp->_vm_contextp__->assertOnGet(1, 1)) {
        if (VL_UNLIKELY(((1U & (~ ((~ (IData)(vlSelfRef.__Vsampled_TOP__rst_n)) 
                                   | VL_ONEHOT0_I((IData)(vlSelfRef.__Vsampled_TOP__grant)))))))) {
            VL_WRITEF_NX("[%0t] %%Error: arbiter.sv:49: Assertion failed in %Narbiter: arbiter granted more than one requester\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name());
            VL_STOP_MT("rtl/arbiter.sv", 49, "");
        }
    }
    if (vlSymsp->_vm_contextp__->assertOnGet(1, 1)) {
        if (VL_UNLIKELY(((1U & (~ ((~ (IData)(vlSelfRef.__Vsampled_TOP__rst_n)) 
                                   | ((0U != (IData)(vlSelfRef.__Vsampled_TOP__req)) 
                                      == (IData)(vlSelfRef.__Vsampled_TOP__valid)))))))) {
            VL_WRITEF_NX("[%0t] %%Error: arbiter.sv:59: Assertion failed in %Narbiter: arbiter valid does not track req\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name());
            VL_STOP_MT("rtl/arbiter.sv", 59, "");
        }
    }
    if (vlSymsp->_vm_contextp__->assertOnGet(1, 1)) {
        if (VL_UNLIKELY(((1U & (~ ((~ (IData)(vlSelfRef.__Vsampled_TOP__rst_n)) 
                                   | (0U == ((IData)(vlSelfRef.__Vsampled_TOP__grant) 
                                             & (~ (IData)(vlSelfRef.__Vsampled_TOP__req)))))))))) {
            VL_WRITEF_NX("[%0t] %%Error: arbiter.sv:54: Assertion failed in %Narbiter: arbiter granted an idle requester\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name());
            VL_STOP_MT("rtl/arbiter.sv", 54, "");
        }
    }
}
