// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vskid_buffer.h for the primary calling header

#include "Vskid_buffer__pch.h"
#include "Vskid_buffer___024root.h"

VL_ATTR_COLD void Vskid_buffer___024root___eval_static(Vskid_buffer___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vskid_buffer___024root___eval_static\n"); );
    Vskid_buffer__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__Vtrigprevexpr___TOP__clk__0 = vlSelfRef.clk;
    vlSelfRef.__Vtrigprevexpr___TOP__rst_n__0 = vlSelfRef.rst_n;
}

VL_ATTR_COLD void Vskid_buffer___024root___eval_initial(Vskid_buffer___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vskid_buffer___024root___eval_initial\n"); );
    Vskid_buffer__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

VL_ATTR_COLD void Vskid_buffer___024root___eval_final(Vskid_buffer___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vskid_buffer___024root___eval_final\n"); );
    Vskid_buffer__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vskid_buffer___024root___dump_triggers__stl(Vskid_buffer___024root* vlSelf);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vskid_buffer___024root___eval_phase__stl(Vskid_buffer___024root* vlSelf);

VL_ATTR_COLD void Vskid_buffer___024root___eval_settle(Vskid_buffer___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vskid_buffer___024root___eval_settle\n"); );
    Vskid_buffer__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    IData/*31:0*/ __VstlIterCount;
    CData/*0:0*/ __VstlContinue;
    // Body
    __VstlIterCount = 0U;
    vlSelfRef.__VstlFirstIteration = 1U;
    __VstlContinue = 1U;
    while (__VstlContinue) {
        if (VL_UNLIKELY(((0x64U < __VstlIterCount)))) {
#ifdef VL_DEBUG
            Vskid_buffer___024root___dump_triggers__stl(vlSelf);
#endif
            VL_FATAL_MT("rtl/skid_buffer.sv", 9, "", "Settle region did not converge.");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
        __VstlContinue = 0U;
        if (Vskid_buffer___024root___eval_phase__stl(vlSelf)) {
            __VstlContinue = 1U;
        }
        vlSelfRef.__VstlFirstIteration = 0U;
    }
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vskid_buffer___024root___dump_triggers__stl(Vskid_buffer___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vskid_buffer___024root___dump_triggers__stl\n"); );
    Vskid_buffer__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U & (~ vlSelfRef.__VstlTriggered.any()))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelfRef.__VstlTriggered.word(0U))) {
        VL_DBG_MSGF("         'stl' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vskid_buffer___024root___stl_sequent__TOP__0(Vskid_buffer___024root* vlSelf);

VL_ATTR_COLD void Vskid_buffer___024root___eval_stl(Vskid_buffer___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vskid_buffer___024root___eval_stl\n"); );
    Vskid_buffer__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VstlTriggered.word(0U))) {
        Vskid_buffer___024root___stl_sequent__TOP__0(vlSelf);
    }
}

VL_ATTR_COLD void Vskid_buffer___024root___stl_sequent__TOP__0(Vskid_buffer___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vskid_buffer___024root___stl_sequent__TOP__0\n"); );
    Vskid_buffer__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.m_valid = vlSelfRef.skid_buffer__DOT__out_valid;
    vlSelfRef.m_data = vlSelfRef.skid_buffer__DOT__out_data;
    vlSelfRef.s_ready = (1U & (~ (IData)(vlSelfRef.skid_buffer__DOT__skid_valid)));
    vlSelfRef.skid_buffer__DOT__accept = ((IData)(vlSelfRef.s_ready) 
                                          & (IData)(vlSelfRef.s_valid));
}

VL_ATTR_COLD void Vskid_buffer___024root___eval_triggers__stl(Vskid_buffer___024root* vlSelf);

VL_ATTR_COLD bool Vskid_buffer___024root___eval_phase__stl(Vskid_buffer___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vskid_buffer___024root___eval_phase__stl\n"); );
    Vskid_buffer__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*0:0*/ __VstlExecute;
    // Body
    Vskid_buffer___024root___eval_triggers__stl(vlSelf);
    __VstlExecute = vlSelfRef.__VstlTriggered.any();
    if (__VstlExecute) {
        Vskid_buffer___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vskid_buffer___024root___dump_triggers__ico(Vskid_buffer___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vskid_buffer___024root___dump_triggers__ico\n"); );
    Vskid_buffer__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U & (~ vlSelfRef.__VicoTriggered.any()))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelfRef.__VicoTriggered.word(0U))) {
        VL_DBG_MSGF("         'ico' region trigger index 0 is active: Internal 'ico' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

#ifdef VL_DEBUG
VL_ATTR_COLD void Vskid_buffer___024root___dump_triggers__act(Vskid_buffer___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vskid_buffer___024root___dump_triggers__act\n"); );
    Vskid_buffer__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U & (~ vlSelfRef.__VactTriggered.any()))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelfRef.__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 0 is active: @(posedge clk)\n");
    }
    if ((2ULL & vlSelfRef.__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 1 is active: @(negedge rst_n)\n");
    }
}
#endif  // VL_DEBUG

#ifdef VL_DEBUG
VL_ATTR_COLD void Vskid_buffer___024root___dump_triggers__nba(Vskid_buffer___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vskid_buffer___024root___dump_triggers__nba\n"); );
    Vskid_buffer__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U & (~ vlSelfRef.__VnbaTriggered.any()))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 0 is active: @(posedge clk)\n");
    }
    if ((2ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 1 is active: @(negedge rst_n)\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vskid_buffer___024root___ctor_var_reset(Vskid_buffer___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vskid_buffer___024root___ctor_var_reset\n"); );
    Vskid_buffer__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->name());
    vlSelf->clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16707436170211756652ull);
    vlSelf->rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1638864771569018232ull);
    vlSelf->s_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3620650391335490897ull);
    vlSelf->s_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 869066129896787687ull);
    vlSelf->s_data = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 3556786919841555277ull);
    vlSelf->m_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8711207929187084452ull);
    vlSelf->m_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15810413859825986029ull);
    vlSelf->m_data = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 13032575655833598985ull);
    vlSelf->skid_buffer__DOT__skid_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17688532644479875701ull);
    vlSelf->skid_buffer__DOT__skid_data = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 10675785715297787693ull);
    vlSelf->skid_buffer__DOT__out_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5214533655799263919ull);
    vlSelf->skid_buffer__DOT__out_data = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 298150333019712063ull);
    vlSelf->skid_buffer__DOT__accept = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8603310481345981209ull);
    vlSelf->skid_buffer__DOT___Vpast_0_0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9204211260244026475ull);
    vlSelf->skid_buffer__DOT___Vpast_1_0 = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 13739741089024825054ull);
    vlSelf->__Vsampled_TOP__rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 725478800516634689ull);
    vlSelf->__Vsampled_TOP__skid_buffer__DOT___Vpast_0_0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15577202937189066849ull);
    vlSelf->__Vsampled_TOP__m_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9199315017708557586ull);
    vlSelf->__Vsampled_TOP__skid_buffer__DOT___Vpast_1_0 = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 2561351073832993860ull);
    vlSelf->__Vsampled_TOP__m_data = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 11537210291347542398ull);
    vlSelf->__Vsampled_TOP__skid_buffer__DOT__skid_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6933316612206883251ull);
    vlSelf->__Vsampled_TOP__skid_buffer__DOT__out_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14923486070199919828ull);
    vlSelf->__Vsampled_TOP__m_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8863178724194893107ull);
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9526919608049418986ull);
    vlSelf->__Vtrigprevexpr___TOP__rst_n__0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14803524876191471008ull);
}
