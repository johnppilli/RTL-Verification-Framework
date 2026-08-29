// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Varbiter.h for the primary calling header

#include "Varbiter__pch.h"
#include "Varbiter___024root.h"

void Varbiter___024root___ico_sequent__TOP__0(Varbiter___024root* vlSelf);

void Varbiter___024root___eval_ico(Varbiter___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Varbiter___024root___eval_ico\n"); );
    Varbiter__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VicoTriggered.word(0U))) {
        Varbiter___024root___ico_sequent__TOP__0(vlSelf);
    }
}

VL_INLINE_OPT void Varbiter___024root___ico_sequent__TOP__0(Varbiter___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Varbiter___024root___ico_sequent__TOP__0\n"); );
    Varbiter__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.grant = 0U;
    vlSelfRef.arbiter__DOT__winner = vlSelfRef.arbiter__DOT__ptr;
    vlSelfRef.arbiter__DOT__unnamedblk1__DOT__unnamedblk2__DOT__idx 
        = (3U & VL_MODDIVS_III(32, (IData)(vlSelfRef.arbiter__DOT__ptr), (IData)(4U)));
    if (((0U == (IData)(vlSelfRef.grant)) & ((IData)(vlSelfRef.req) 
                                             >> (IData)(vlSelfRef.arbiter__DOT__unnamedblk1__DOT__unnamedblk2__DOT__idx)))) {
        vlSelfRef.grant = ((IData)(vlSelfRef.grant) 
                           | (0xfU & ((IData)(1U) << (IData)(vlSelfRef.arbiter__DOT__unnamedblk1__DOT__unnamedblk2__DOT__idx))));
        vlSelfRef.arbiter__DOT__winner = vlSelfRef.arbiter__DOT__unnamedblk1__DOT__unnamedblk2__DOT__idx;
    }
    vlSelfRef.arbiter__DOT__unnamedblk1__DOT__unnamedblk2__DOT__idx 
        = (3U & VL_MODDIVS_III(32, ((IData)(1U) + (IData)(vlSelfRef.arbiter__DOT__ptr)), (IData)(4U)));
    if (((0U == (IData)(vlSelfRef.grant)) & ((IData)(vlSelfRef.req) 
                                             >> (IData)(vlSelfRef.arbiter__DOT__unnamedblk1__DOT__unnamedblk2__DOT__idx)))) {
        vlSelfRef.grant = ((IData)(vlSelfRef.grant) 
                           | (0xfU & ((IData)(1U) << (IData)(vlSelfRef.arbiter__DOT__unnamedblk1__DOT__unnamedblk2__DOT__idx))));
        vlSelfRef.arbiter__DOT__winner = vlSelfRef.arbiter__DOT__unnamedblk1__DOT__unnamedblk2__DOT__idx;
    }
    vlSelfRef.arbiter__DOT__unnamedblk1__DOT__unnamedblk2__DOT__idx 
        = (3U & VL_MODDIVS_III(32, ((IData)(2U) + (IData)(vlSelfRef.arbiter__DOT__ptr)), (IData)(4U)));
    if (((0U == (IData)(vlSelfRef.grant)) & ((IData)(vlSelfRef.req) 
                                             >> (IData)(vlSelfRef.arbiter__DOT__unnamedblk1__DOT__unnamedblk2__DOT__idx)))) {
        vlSelfRef.grant = ((IData)(vlSelfRef.grant) 
                           | (0xfU & ((IData)(1U) << (IData)(vlSelfRef.arbiter__DOT__unnamedblk1__DOT__unnamedblk2__DOT__idx))));
        vlSelfRef.arbiter__DOT__winner = vlSelfRef.arbiter__DOT__unnamedblk1__DOT__unnamedblk2__DOT__idx;
    }
    vlSelfRef.arbiter__DOT__unnamedblk1__DOT__unnamedblk2__DOT__idx 
        = (3U & VL_MODDIVS_III(32, ((IData)(3U) + (IData)(vlSelfRef.arbiter__DOT__ptr)), (IData)(4U)));
    if (((0U == (IData)(vlSelfRef.grant)) & ((IData)(vlSelfRef.req) 
                                             >> (IData)(vlSelfRef.arbiter__DOT__unnamedblk1__DOT__unnamedblk2__DOT__idx)))) {
        vlSelfRef.grant = ((IData)(vlSelfRef.grant) 
                           | (0xfU & ((IData)(1U) << (IData)(vlSelfRef.arbiter__DOT__unnamedblk1__DOT__unnamedblk2__DOT__idx))));
        vlSelfRef.arbiter__DOT__winner = vlSelfRef.arbiter__DOT__unnamedblk1__DOT__unnamedblk2__DOT__idx;
    }
    vlSelfRef.valid = (0U != (IData)(vlSelfRef.grant));
}

void Varbiter___024root___eval_triggers__ico(Varbiter___024root* vlSelf);

bool Varbiter___024root___eval_phase__ico(Varbiter___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Varbiter___024root___eval_phase__ico\n"); );
    Varbiter__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*0:0*/ __VicoExecute;
    // Body
    Varbiter___024root___eval_triggers__ico(vlSelf);
    __VicoExecute = vlSelfRef.__VicoTriggered.any();
    if (__VicoExecute) {
        Varbiter___024root___eval_ico(vlSelf);
    }
    return (__VicoExecute);
}

void Varbiter___024root___eval_act(Varbiter___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Varbiter___024root___eval_act\n"); );
    Varbiter__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

void Varbiter___024root___nba_sequent__TOP__0(Varbiter___024root* vlSelf);
void Varbiter___024root___nba_sequent__TOP__1(Varbiter___024root* vlSelf);

void Varbiter___024root___eval_nba(Varbiter___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Varbiter___024root___eval_nba\n"); );
    Varbiter__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Varbiter___024root___nba_sequent__TOP__0(vlSelf);
    }
    if ((3ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Varbiter___024root___nba_sequent__TOP__1(vlSelf);
    }
}

VL_INLINE_OPT void Varbiter___024root___nba_sequent__TOP__1(Varbiter___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Varbiter___024root___nba_sequent__TOP__1\n"); );
    Varbiter__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (vlSelfRef.rst_n) {
        if (vlSelfRef.valid) {
            vlSelfRef.arbiter__DOT__ptr = ((3U == (IData)(vlSelfRef.arbiter__DOT__winner))
                                            ? 0U : 
                                           (3U & ((IData)(1U) 
                                                  + (IData)(vlSelfRef.arbiter__DOT__winner))));
        }
    } else {
        vlSelfRef.arbiter__DOT__ptr = 0U;
    }
    vlSelfRef.grant = 0U;
    vlSelfRef.arbiter__DOT__winner = vlSelfRef.arbiter__DOT__ptr;
    vlSelfRef.arbiter__DOT__unnamedblk1__DOT__unnamedblk2__DOT__idx 
        = (3U & VL_MODDIVS_III(32, (IData)(vlSelfRef.arbiter__DOT__ptr), (IData)(4U)));
    if (((0U == (IData)(vlSelfRef.grant)) & ((IData)(vlSelfRef.req) 
                                             >> (IData)(vlSelfRef.arbiter__DOT__unnamedblk1__DOT__unnamedblk2__DOT__idx)))) {
        vlSelfRef.grant = ((IData)(vlSelfRef.grant) 
                           | (0xfU & ((IData)(1U) << (IData)(vlSelfRef.arbiter__DOT__unnamedblk1__DOT__unnamedblk2__DOT__idx))));
        vlSelfRef.arbiter__DOT__winner = vlSelfRef.arbiter__DOT__unnamedblk1__DOT__unnamedblk2__DOT__idx;
    }
    vlSelfRef.arbiter__DOT__unnamedblk1__DOT__unnamedblk2__DOT__idx 
        = (3U & VL_MODDIVS_III(32, ((IData)(1U) + (IData)(vlSelfRef.arbiter__DOT__ptr)), (IData)(4U)));
    if (((0U == (IData)(vlSelfRef.grant)) & ((IData)(vlSelfRef.req) 
                                             >> (IData)(vlSelfRef.arbiter__DOT__unnamedblk1__DOT__unnamedblk2__DOT__idx)))) {
        vlSelfRef.grant = ((IData)(vlSelfRef.grant) 
                           | (0xfU & ((IData)(1U) << (IData)(vlSelfRef.arbiter__DOT__unnamedblk1__DOT__unnamedblk2__DOT__idx))));
        vlSelfRef.arbiter__DOT__winner = vlSelfRef.arbiter__DOT__unnamedblk1__DOT__unnamedblk2__DOT__idx;
    }
    vlSelfRef.arbiter__DOT__unnamedblk1__DOT__unnamedblk2__DOT__idx 
        = (3U & VL_MODDIVS_III(32, ((IData)(2U) + (IData)(vlSelfRef.arbiter__DOT__ptr)), (IData)(4U)));
    if (((0U == (IData)(vlSelfRef.grant)) & ((IData)(vlSelfRef.req) 
                                             >> (IData)(vlSelfRef.arbiter__DOT__unnamedblk1__DOT__unnamedblk2__DOT__idx)))) {
        vlSelfRef.grant = ((IData)(vlSelfRef.grant) 
                           | (0xfU & ((IData)(1U) << (IData)(vlSelfRef.arbiter__DOT__unnamedblk1__DOT__unnamedblk2__DOT__idx))));
        vlSelfRef.arbiter__DOT__winner = vlSelfRef.arbiter__DOT__unnamedblk1__DOT__unnamedblk2__DOT__idx;
    }
    vlSelfRef.arbiter__DOT__unnamedblk1__DOT__unnamedblk2__DOT__idx 
        = (3U & VL_MODDIVS_III(32, ((IData)(3U) + (IData)(vlSelfRef.arbiter__DOT__ptr)), (IData)(4U)));
    if (((0U == (IData)(vlSelfRef.grant)) & ((IData)(vlSelfRef.req) 
                                             >> (IData)(vlSelfRef.arbiter__DOT__unnamedblk1__DOT__unnamedblk2__DOT__idx)))) {
        vlSelfRef.grant = ((IData)(vlSelfRef.grant) 
                           | (0xfU & ((IData)(1U) << (IData)(vlSelfRef.arbiter__DOT__unnamedblk1__DOT__unnamedblk2__DOT__idx))));
        vlSelfRef.arbiter__DOT__winner = vlSelfRef.arbiter__DOT__unnamedblk1__DOT__unnamedblk2__DOT__idx;
    }
    vlSelfRef.valid = (0U != (IData)(vlSelfRef.grant));
}

void Varbiter___024root___eval_triggers__act(Varbiter___024root* vlSelf);

bool Varbiter___024root___eval_phase__act(Varbiter___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Varbiter___024root___eval_phase__act\n"); );
    Varbiter__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    VlTriggerVec<2> __VpreTriggered;
    CData/*0:0*/ __VactExecute;
    // Body
    Varbiter___024root___eval_triggers__act(vlSelf);
    __VactExecute = vlSelfRef.__VactTriggered.any();
    if (__VactExecute) {
        __VpreTriggered.andNot(vlSelfRef.__VactTriggered, vlSelfRef.__VnbaTriggered);
        vlSelfRef.__VnbaTriggered.thisOr(vlSelfRef.__VactTriggered);
        Varbiter___024root___eval_act(vlSelf);
    }
    return (__VactExecute);
}

bool Varbiter___024root___eval_phase__nba(Varbiter___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Varbiter___024root___eval_phase__nba\n"); );
    Varbiter__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = vlSelfRef.__VnbaTriggered.any();
    if (__VnbaExecute) {
        Varbiter___024root___eval_nba(vlSelf);
        vlSelfRef.__VnbaTriggered.clear();
    }
    return (__VnbaExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Varbiter___024root___dump_triggers__ico(Varbiter___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Varbiter___024root___dump_triggers__nba(Varbiter___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Varbiter___024root___dump_triggers__act(Varbiter___024root* vlSelf);
#endif  // VL_DEBUG

void Varbiter___024root___eval(Varbiter___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Varbiter___024root___eval\n"); );
    Varbiter__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    vlSelfRef.__Vsampled_TOP__rst_n = vlSelfRef.rst_n;
    vlSelfRef.__Vsampled_TOP__grant = vlSelfRef.grant;
    vlSelfRef.__Vsampled_TOP__req = vlSelfRef.req;
    vlSelfRef.__Vsampled_TOP__valid = vlSelfRef.valid;
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
            Varbiter___024root___dump_triggers__ico(vlSelf);
#endif
            VL_FATAL_MT("rtl/arbiter.sv", 7, "", "Input combinational region did not converge.");
        }
        __VicoIterCount = ((IData)(1U) + __VicoIterCount);
        __VicoContinue = 0U;
        if (Varbiter___024root___eval_phase__ico(vlSelf)) {
            __VicoContinue = 1U;
        }
        vlSelfRef.__VicoFirstIteration = 0U;
    }
    __VnbaIterCount = 0U;
    __VnbaContinue = 1U;
    while (__VnbaContinue) {
        if (VL_UNLIKELY(((0x64U < __VnbaIterCount)))) {
#ifdef VL_DEBUG
            Varbiter___024root___dump_triggers__nba(vlSelf);
#endif
            VL_FATAL_MT("rtl/arbiter.sv", 7, "", "NBA region did not converge.");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        __VnbaContinue = 0U;
        vlSelfRef.__VactIterCount = 0U;
        vlSelfRef.__VactContinue = 1U;
        while (vlSelfRef.__VactContinue) {
            if (VL_UNLIKELY(((0x64U < vlSelfRef.__VactIterCount)))) {
#ifdef VL_DEBUG
                Varbiter___024root___dump_triggers__act(vlSelf);
#endif
                VL_FATAL_MT("rtl/arbiter.sv", 7, "", "Active region did not converge.");
            }
            vlSelfRef.__VactIterCount = ((IData)(1U) 
                                         + vlSelfRef.__VactIterCount);
            vlSelfRef.__VactContinue = 0U;
            if (Varbiter___024root___eval_phase__act(vlSelf)) {
                vlSelfRef.__VactContinue = 1U;
            }
        }
        if (Varbiter___024root___eval_phase__nba(vlSelf)) {
            __VnbaContinue = 1U;
        }
    }
}

#ifdef VL_DEBUG
void Varbiter___024root___eval_debug_assertions(Varbiter___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Varbiter___024root___eval_debug_assertions\n"); );
    Varbiter__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (VL_UNLIKELY(((vlSelfRef.clk & 0xfeU)))) {
        Verilated::overWidthError("clk");}
    if (VL_UNLIKELY(((vlSelfRef.rst_n & 0xfeU)))) {
        Verilated::overWidthError("rst_n");}
    if (VL_UNLIKELY(((vlSelfRef.req & 0xf0U)))) {
        Verilated::overWidthError("req");}
}
#endif  // VL_DEBUG
