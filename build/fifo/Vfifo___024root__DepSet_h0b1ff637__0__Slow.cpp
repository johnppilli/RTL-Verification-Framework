// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vfifo.h for the primary calling header

#include "Vfifo__pch.h"
#include "Vfifo___024root.h"

VL_ATTR_COLD void Vfifo___024root___eval_static(Vfifo___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfifo___024root___eval_static\n"); );
    Vfifo__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__Vtrigprevexpr___TOP__clk__0 = vlSelfRef.clk;
    vlSelfRef.__Vtrigprevexpr___TOP__rst_n__0 = vlSelfRef.rst_n;
}

VL_ATTR_COLD void Vfifo___024root___eval_initial(Vfifo___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfifo___024root___eval_initial\n"); );
    Vfifo__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

VL_ATTR_COLD void Vfifo___024root___eval_final(Vfifo___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfifo___024root___eval_final\n"); );
    Vfifo__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vfifo___024root___dump_triggers__stl(Vfifo___024root* vlSelf);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vfifo___024root___eval_phase__stl(Vfifo___024root* vlSelf);

VL_ATTR_COLD void Vfifo___024root___eval_settle(Vfifo___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfifo___024root___eval_settle\n"); );
    Vfifo__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
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
            Vfifo___024root___dump_triggers__stl(vlSelf);
#endif
            VL_FATAL_MT("rtl/fifo.sv", 8, "", "Settle region did not converge.");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
        __VstlContinue = 0U;
        if (Vfifo___024root___eval_phase__stl(vlSelf)) {
            __VstlContinue = 1U;
        }
        vlSelfRef.__VstlFirstIteration = 0U;
    }
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vfifo___024root___dump_triggers__stl(Vfifo___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfifo___024root___dump_triggers__stl\n"); );
    Vfifo__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
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

VL_ATTR_COLD void Vfifo___024root___stl_sequent__TOP__0(Vfifo___024root* vlSelf);

VL_ATTR_COLD void Vfifo___024root___eval_stl(Vfifo___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfifo___024root___eval_stl\n"); );
    Vfifo__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VstlTriggered.word(0U))) {
        Vfifo___024root___stl_sequent__TOP__0(vlSelf);
    }
}

VL_ATTR_COLD void Vfifo___024root___stl_sequent__TOP__0(Vfifo___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfifo___024root___stl_sequent__TOP__0\n"); );
    Vfifo__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.count = vlSelfRef.fifo__DOT__count_reg;
    vlSelfRef.data_out = vlSelfRef.fifo__DOT__memory
        [vlSelfRef.fifo__DOT__rd_ptr];
    vlSelfRef.full = (8U == (IData)(vlSelfRef.fifo__DOT__count_reg));
    vlSelfRef.empty = (0U == (IData)(vlSelfRef.fifo__DOT__count_reg));
    vlSelfRef.fifo__DOT__do_write = ((~ (IData)(vlSelfRef.full)) 
                                     & (IData)(vlSelfRef.wr_en));
    vlSelfRef.fifo__DOT__do_read = ((~ (IData)(vlSelfRef.empty)) 
                                    & (IData)(vlSelfRef.rd_en));
}

VL_ATTR_COLD void Vfifo___024root___eval_triggers__stl(Vfifo___024root* vlSelf);

VL_ATTR_COLD bool Vfifo___024root___eval_phase__stl(Vfifo___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfifo___024root___eval_phase__stl\n"); );
    Vfifo__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*0:0*/ __VstlExecute;
    // Body
    Vfifo___024root___eval_triggers__stl(vlSelf);
    __VstlExecute = vlSelfRef.__VstlTriggered.any();
    if (__VstlExecute) {
        Vfifo___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vfifo___024root___dump_triggers__ico(Vfifo___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfifo___024root___dump_triggers__ico\n"); );
    Vfifo__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
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
VL_ATTR_COLD void Vfifo___024root___dump_triggers__act(Vfifo___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfifo___024root___dump_triggers__act\n"); );
    Vfifo__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
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
VL_ATTR_COLD void Vfifo___024root___dump_triggers__nba(Vfifo___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfifo___024root___dump_triggers__nba\n"); );
    Vfifo__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
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

VL_ATTR_COLD void Vfifo___024root___ctor_var_reset(Vfifo___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfifo___024root___ctor_var_reset\n"); );
    Vfifo__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->name());
    vlSelf->clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16707436170211756652ull);
    vlSelf->rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1638864771569018232ull);
    vlSelf->wr_en = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7710928637576349896ull);
    vlSelf->data_in = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 10574596302020702150ull);
    vlSelf->rd_en = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3814484142505630662ull);
    vlSelf->data_out = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 11675680895196038875ull);
    vlSelf->full = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6695099141381822181ull);
    vlSelf->empty = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3016723684638320966ull);
    vlSelf->count = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 10730142128712957955ull);
    for (int __Vi0 = 0; __Vi0 < 8; ++__Vi0) {
        vlSelf->fifo__DOT__memory[__Vi0] = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 6800003246615495397ull);
    }
    vlSelf->fifo__DOT__wr_ptr = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 7427304662663069374ull);
    vlSelf->fifo__DOT__rd_ptr = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 8995441205502530458ull);
    vlSelf->fifo__DOT__count_reg = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 15066520538175484698ull);
    vlSelf->fifo__DOT__do_write = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9931903588065754062ull);
    vlSelf->fifo__DOT__do_read = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17365508771344304525ull);
    vlSelf->fifo__DOT___Vpast_0_0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3830305727071004900ull);
    vlSelf->fifo__DOT___Vpast_1_0 = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 1404744488812273671ull);
    vlSelf->__Vsampled_TOP__rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 725478800516634689ull);
    vlSelf->__Vsampled_TOP__fifo__DOT__count_reg = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 17450908821849424762ull);
    vlSelf->__Vsampled_TOP__full = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4675967105019186489ull);
    vlSelf->__Vsampled_TOP__empty = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15890603821079200336ull);
    vlSelf->__Vsampled_TOP__fifo__DOT___Vpast_0_0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16379016192669845302ull);
    vlSelf->__Vsampled_TOP__fifo__DOT___Vpast_1_0 = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 12901213795625616693ull);
    vlSelf->__Vsampled_TOP__fifo__DOT__do_write = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3230967951189917987ull);
    vlSelf->__Vsampled_TOP__fifo__DOT__do_read = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5459175501022335757ull);
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9526919608049418986ull);
    vlSelf->__Vtrigprevexpr___TOP__rst_n__0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14803524876191471008ull);
    for (int __Vi0 = 0; __Vi0 < 2; ++__Vi0) {
        vlSelf->__Vm_traceActivity[__Vi0] = 0;
    }
}
