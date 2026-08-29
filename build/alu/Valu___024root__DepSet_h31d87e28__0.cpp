// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Valu.h for the primary calling header

#include "Valu__pch.h"
#include "Valu__Syms.h"
#include "Valu___024root.h"

#ifdef VL_DEBUG
VL_ATTR_COLD void Valu___024root___dump_triggers__ico(Valu___024root* vlSelf);
#endif  // VL_DEBUG

void Valu___024root___eval_triggers__ico(Valu___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Valu___024root___eval_triggers__ico\n"); );
    Valu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VicoTriggered.setBit(0U, (IData)(vlSelfRef.__VicoFirstIteration));
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Valu___024root___dump_triggers__ico(vlSelf);
    }
#endif
}

VL_INLINE_OPT void Valu___024root___ico_sequent__TOP__0(Valu___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Valu___024root___ico_sequent__TOP__0\n"); );
    Valu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*0:0*/ __VdfgRegularize_h0dff6736_0_2;
    __VdfgRegularize_h0dff6736_0_2 = 0;
    // Body
    if ((1U & (~ VL_ONEHOT_I((((1U == (IData)(vlSelfRef.op)) 
                               << 1U) | (0U == (IData)(vlSelfRef.op))))))) {
        if ((0U != (((1U == (IData)(vlSelfRef.op)) 
                     << 1U) | (0U == (IData)(vlSelfRef.op))))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: alu.sv:60: Assertion failed in %Nalu: unique case, but multiple matches found for '4'h%x'\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name(),4,
                             (IData)(vlSelfRef.op));
                VL_STOP_MT("rtl/alu.sv", 60, "");
            }
        }
    }
    vlSelfRef.alu__DOT__sum = (0x1ffffffffULL & ((QData)((IData)(vlSelfRef.a)) 
                                                 + (QData)((IData)(vlSelfRef.b))));
    vlSelfRef.alu__DOT__diff = (0x1ffffffffULL & ((QData)((IData)(vlSelfRef.a)) 
                                                  - (QData)((IData)(vlSelfRef.b))));
    vlSelfRef.carry = 0U;
    vlSelfRef.result = 0U;
    if ((8U & (IData)(vlSelfRef.op))) {
        if ((4U & (IData)(vlSelfRef.op))) {
            vlSelfRef.carry = 0U;
            vlSelfRef.result = 0U;
        } else if ((2U & (IData)(vlSelfRef.op))) {
            vlSelfRef.carry = 0U;
            vlSelfRef.result = 0U;
        } else {
            vlSelfRef.result = ((1U & (IData)(vlSelfRef.op))
                                 ? ((vlSelfRef.a < vlSelfRef.b)
                                     ? 1U : 0U) : (
                                                   VL_LTS_III(32, vlSelfRef.a, vlSelfRef.b)
                                                    ? 1U
                                                    : 0U));
        }
    } else {
        if ((1U & (~ ((IData)(vlSelfRef.op) >> 2U)))) {
            if ((1U & (~ ((IData)(vlSelfRef.op) >> 1U)))) {
                vlSelfRef.carry = (1U & ((1U & (IData)(vlSelfRef.op))
                                          ? (~ (IData)(
                                                       (vlSelfRef.alu__DOT__diff 
                                                        >> 0x20U)))
                                          : (IData)(
                                                    (vlSelfRef.alu__DOT__sum 
                                                     >> 0x20U))));
            }
        }
        vlSelfRef.result = ((4U & (IData)(vlSelfRef.op))
                             ? ((2U & (IData)(vlSelfRef.op))
                                 ? ((1U & (IData)(vlSelfRef.op))
                                     ? VL_SHIFTRS_III(32,32,5, vlSelfRef.a, 
                                                      (0x1fU 
                                                       & vlSelfRef.b))
                                     : (vlSelfRef.a 
                                        >> (0x1fU & vlSelfRef.b)))
                                 : ((1U & (IData)(vlSelfRef.op))
                                     ? (vlSelfRef.a 
                                        << (0x1fU & vlSelfRef.b))
                                     : (vlSelfRef.a 
                                        ^ vlSelfRef.b)))
                             : ((2U & (IData)(vlSelfRef.op))
                                 ? ((1U & (IData)(vlSelfRef.op))
                                     ? (vlSelfRef.a 
                                        | vlSelfRef.b)
                                     : (vlSelfRef.a 
                                        & vlSelfRef.b))
                                 : ((1U & (IData)(vlSelfRef.op))
                                     ? (IData)(vlSelfRef.alu__DOT__diff)
                                     : (IData)(vlSelfRef.alu__DOT__sum))));
    }
    vlSelfRef.zero = (0U == vlSelfRef.result);
    vlSelfRef.negative = (vlSelfRef.result >> 0x1fU);
    __VdfgRegularize_h0dff6736_0_2 = ((IData)(vlSelfRef.negative) 
                                      != (vlSelfRef.a 
                                          >> 0x1fU));
    vlSelfRef.overflow = ((0U == (IData)(vlSelfRef.op))
                           ? (((vlSelfRef.a >> 0x1fU) 
                               == (vlSelfRef.b >> 0x1fU)) 
                              & (IData)(__VdfgRegularize_h0dff6736_0_2))
                           : ((1U == (IData)(vlSelfRef.op)) 
                              & (((vlSelfRef.a >> 0x1fU) 
                                  != (vlSelfRef.b >> 0x1fU)) 
                                 & (IData)(__VdfgRegularize_h0dff6736_0_2))));
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Valu___024root___dump_triggers__act(Valu___024root* vlSelf);
#endif  // VL_DEBUG

void Valu___024root___eval_triggers__act(Valu___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Valu___024root___eval_triggers__act\n"); );
    Valu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Valu___024root___dump_triggers__act(vlSelf);
    }
#endif
}
