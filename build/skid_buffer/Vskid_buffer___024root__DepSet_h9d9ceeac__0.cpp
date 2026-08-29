// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vskid_buffer.h for the primary calling header

#include "Vskid_buffer__pch.h"
#include "Vskid_buffer___024root.h"

void Vskid_buffer___024root___ico_sequent__TOP__0(Vskid_buffer___024root* vlSelf);

void Vskid_buffer___024root___eval_ico(Vskid_buffer___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vskid_buffer___024root___eval_ico\n"); );
    Vskid_buffer__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VicoTriggered.word(0U))) {
        Vskid_buffer___024root___ico_sequent__TOP__0(vlSelf);
    }
}

VL_INLINE_OPT void Vskid_buffer___024root___ico_sequent__TOP__0(Vskid_buffer___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vskid_buffer___024root___ico_sequent__TOP__0\n"); );
    Vskid_buffer__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.skid_buffer__DOT__accept = ((IData)(vlSelfRef.s_ready) 
                                          & (IData)(vlSelfRef.s_valid));
}

void Vskid_buffer___024root___eval_triggers__ico(Vskid_buffer___024root* vlSelf);

bool Vskid_buffer___024root___eval_phase__ico(Vskid_buffer___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vskid_buffer___024root___eval_phase__ico\n"); );
    Vskid_buffer__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*0:0*/ __VicoExecute;
    // Body
    Vskid_buffer___024root___eval_triggers__ico(vlSelf);
    __VicoExecute = vlSelfRef.__VicoTriggered.any();
    if (__VicoExecute) {
        Vskid_buffer___024root___eval_ico(vlSelf);
    }
    return (__VicoExecute);
}

void Vskid_buffer___024root___eval_act(Vskid_buffer___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vskid_buffer___024root___eval_act\n"); );
    Vskid_buffer__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

void Vskid_buffer___024root___nba_sequent__TOP__0(Vskid_buffer___024root* vlSelf);
void Vskid_buffer___024root___nba_sequent__TOP__1(Vskid_buffer___024root* vlSelf);

void Vskid_buffer___024root___eval_nba(Vskid_buffer___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vskid_buffer___024root___eval_nba\n"); );
    Vskid_buffer__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vskid_buffer___024root___nba_sequent__TOP__0(vlSelf);
    }
    if ((3ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vskid_buffer___024root___nba_sequent__TOP__1(vlSelf);
    }
}

VL_INLINE_OPT void Vskid_buffer___024root___nba_sequent__TOP__1(Vskid_buffer___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vskid_buffer___024root___nba_sequent__TOP__1\n"); );
    Vskid_buffer__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*0:0*/ __Vdly__skid_buffer__DOT__out_valid;
    __Vdly__skid_buffer__DOT__out_valid = 0;
    // Body
    __Vdly__skid_buffer__DOT__out_valid = vlSelfRef.skid_buffer__DOT__out_valid;
    if (vlSelfRef.rst_n) {
        if ((1U & ((~ (IData)(vlSelfRef.skid_buffer__DOT__out_valid)) 
                   | (IData)(vlSelfRef.m_ready)))) {
            __Vdly__skid_buffer__DOT__out_valid = ((IData)(vlSelfRef.skid_buffer__DOT__skid_valid) 
                                                   | (IData)(vlSelfRef.skid_buffer__DOT__accept));
        }
        if ((((IData)(vlSelfRef.skid_buffer__DOT__accept) 
              & (IData)(vlSelfRef.skid_buffer__DOT__out_valid)) 
             & (~ (IData)(vlSelfRef.m_ready)))) {
            vlSelfRef.skid_buffer__DOT__skid_valid = 1U;
        } else if (vlSelfRef.m_ready) {
            vlSelfRef.skid_buffer__DOT__skid_valid = 0U;
        }
    } else {
        __Vdly__skid_buffer__DOT__out_valid = 0U;
        vlSelfRef.skid_buffer__DOT__skid_valid = 0U;
    }
    vlSelfRef.skid_buffer__DOT__out_valid = __Vdly__skid_buffer__DOT__out_valid;
    vlSelfRef.m_valid = vlSelfRef.skid_buffer__DOT__out_valid;
    vlSelfRef.s_ready = (1U & (~ (IData)(vlSelfRef.skid_buffer__DOT__skid_valid)));
    vlSelfRef.skid_buffer__DOT__accept = ((IData)(vlSelfRef.s_ready) 
                                          & (IData)(vlSelfRef.s_valid));
}

void Vskid_buffer___024root___eval_triggers__act(Vskid_buffer___024root* vlSelf);

bool Vskid_buffer___024root___eval_phase__act(Vskid_buffer___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vskid_buffer___024root___eval_phase__act\n"); );
    Vskid_buffer__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    VlTriggerVec<2> __VpreTriggered;
    CData/*0:0*/ __VactExecute;
    // Body
    Vskid_buffer___024root___eval_triggers__act(vlSelf);
    __VactExecute = vlSelfRef.__VactTriggered.any();
    if (__VactExecute) {
        __VpreTriggered.andNot(vlSelfRef.__VactTriggered, vlSelfRef.__VnbaTriggered);
        vlSelfRef.__VnbaTriggered.thisOr(vlSelfRef.__VactTriggered);
        Vskid_buffer___024root___eval_act(vlSelf);
    }
    return (__VactExecute);
}

bool Vskid_buffer___024root___eval_phase__nba(Vskid_buffer___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vskid_buffer___024root___eval_phase__nba\n"); );
    Vskid_buffer__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = vlSelfRef.__VnbaTriggered.any();
    if (__VnbaExecute) {
        Vskid_buffer___024root___eval_nba(vlSelf);
        vlSelfRef.__VnbaTriggered.clear();
    }
    return (__VnbaExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vskid_buffer___024root___dump_triggers__ico(Vskid_buffer___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Vskid_buffer___024root___dump_triggers__nba(Vskid_buffer___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Vskid_buffer___024root___dump_triggers__act(Vskid_buffer___024root* vlSelf);
#endif  // VL_DEBUG

void Vskid_buffer___024root___eval(Vskid_buffer___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vskid_buffer___024root___eval\n"); );
    Vskid_buffer__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    vlSelfRef.__Vsampled_TOP__rst_n = vlSelfRef.rst_n;
    vlSelfRef.__Vsampled_TOP__skid_buffer__DOT___Vpast_0_0 
        = vlSelfRef.skid_buffer__DOT___Vpast_0_0;
    vlSelfRef.__Vsampled_TOP__m_valid = vlSelfRef.m_valid;
    vlSelfRef.__Vsampled_TOP__skid_buffer__DOT___Vpast_1_0 
        = vlSelfRef.skid_buffer__DOT___Vpast_1_0;
    vlSelfRef.__Vsampled_TOP__m_data = vlSelfRef.m_data;
    vlSelfRef.__Vsampled_TOP__skid_buffer__DOT__skid_valid 
        = vlSelfRef.skid_buffer__DOT__skid_valid;
    vlSelfRef.__Vsampled_TOP__skid_buffer__DOT__out_valid 
        = vlSelfRef.skid_buffer__DOT__out_valid;
    vlSelfRef.__Vsampled_TOP__m_ready = vlSelfRef.m_ready;
    IData/*31:0*/ __VicoIterCount;
    CData/*0:0*/ __VicoContinue;
    IData/*31:0*/ __VnbaIterCount;
    CData/*0:0*/ __VnbaContinue;
    // Body
    __VicoIterCount = 0U;
    vlSelfRef.__VicoFirstIteration = 1U;
    __VicoContinue = 1U;
    while (__VicoContinue) {
        if (VL_UNLIKELY(((0x64U < __VicoIterCount)))) {
#ifdef VL_DEBUG
            Vskid_buffer___024root___dump_triggers__ico(vlSelf);
#endif
            VL_FATAL_MT("rtl/skid_buffer.sv", 9, "", "Input combinational region did not converge.");
        }
        __VicoIterCount = ((IData)(1U) + __VicoIterCount);
        __VicoContinue = 0U;
        if (Vskid_buffer___024root___eval_phase__ico(vlSelf)) {
            __VicoContinue = 1U;
        }
        vlSelfRef.__VicoFirstIteration = 0U;
    }
    __VnbaIterCount = 0U;
    __VnbaContinue = 1U;
    while (__VnbaContinue) {
        if (VL_UNLIKELY(((0x64U < __VnbaIterCount)))) {
#ifdef VL_DEBUG
            Vskid_buffer___024root___dump_triggers__nba(vlSelf);
#endif
            VL_FATAL_MT("rtl/skid_buffer.sv", 9, "", "NBA region did not converge.");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        __VnbaContinue = 0U;
        vlSelfRef.__VactIterCount = 0U;
        vlSelfRef.__VactContinue = 1U;
        while (vlSelfRef.__VactContinue) {
            if (VL_UNLIKELY(((0x64U < vlSelfRef.__VactIterCount)))) {
#ifdef VL_DEBUG
                Vskid_buffer___024root___dump_triggers__act(vlSelf);
#endif
                VL_FATAL_MT("rtl/skid_buffer.sv", 9, "", "Active region did not converge.");
            }
            vlSelfRef.__VactIterCount = ((IData)(1U) 
                                         + vlSelfRef.__VactIterCount);
            vlSelfRef.__VactContinue = 0U;
            if (Vskid_buffer___024root___eval_phase__act(vlSelf)) {
                vlSelfRef.__VactContinue = 1U;
            }
        }
        if (Vskid_buffer___024root___eval_phase__nba(vlSelf)) {
            __VnbaContinue = 1U;
        }
    }
}

#ifdef VL_DEBUG
void Vskid_buffer___024root___eval_debug_assertions(Vskid_buffer___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vskid_buffer___024root___eval_debug_assertions\n"); );
    Vskid_buffer__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (VL_UNLIKELY(((vlSelfRef.clk & 0xfeU)))) {
        Verilated::overWidthError("clk");}
    if (VL_UNLIKELY(((vlSelfRef.rst_n & 0xfeU)))) {
        Verilated::overWidthError("rst_n");}
    if (VL_UNLIKELY(((vlSelfRef.s_valid & 0xfeU)))) {
        Verilated::overWidthError("s_valid");}
    if (VL_UNLIKELY(((vlSelfRef.m_ready & 0xfeU)))) {
        Verilated::overWidthError("m_ready");}
}
#endif  // VL_DEBUG
