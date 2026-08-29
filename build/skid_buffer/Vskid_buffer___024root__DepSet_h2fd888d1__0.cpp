// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vskid_buffer.h for the primary calling header

#include "Vskid_buffer__pch.h"
#include "Vskid_buffer__Syms.h"
#include "Vskid_buffer___024root.h"

#ifdef VL_DEBUG
VL_ATTR_COLD void Vskid_buffer___024root___dump_triggers__ico(Vskid_buffer___024root* vlSelf);
#endif  // VL_DEBUG

void Vskid_buffer___024root___eval_triggers__ico(Vskid_buffer___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vskid_buffer___024root___eval_triggers__ico\n"); );
    Vskid_buffer__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VicoTriggered.setBit(0U, (IData)(vlSelfRef.__VicoFirstIteration));
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vskid_buffer___024root___dump_triggers__ico(vlSelf);
    }
#endif
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vskid_buffer___024root___dump_triggers__act(Vskid_buffer___024root* vlSelf);
#endif  // VL_DEBUG

void Vskid_buffer___024root___eval_triggers__act(Vskid_buffer___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vskid_buffer___024root___eval_triggers__act\n"); );
    Vskid_buffer__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
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
        Vskid_buffer___024root___dump_triggers__act(vlSelf);
    }
#endif
}

VL_INLINE_OPT void Vskid_buffer___024root___nba_sequent__TOP__0(Vskid_buffer___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vskid_buffer___024root___nba_sequent__TOP__0\n"); );
    Vskid_buffer__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (vlSymsp->_vm_contextp__->assertOnGet(1, 1)) {
        if (VL_UNLIKELY(((1U & (~ ((~ (IData)(vlSelfRef.__Vsampled_TOP__rst_n)) 
                                   | ((~ (IData)(vlSelfRef.__Vsampled_TOP__skid_buffer__DOT__skid_valid)) 
                                      | (IData)(vlSelfRef.__Vsampled_TOP__skid_buffer__DOT__out_valid)))))))) {
            VL_WRITEF_NX("[%0t] %%Error: skid_buffer.sv:74: Assertion failed in %Nskid_buffer: skid_buffer skid occupied with empty output\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name());
            VL_STOP_MT("rtl/skid_buffer.sv", 74, "");
        }
    }
    if (vlSymsp->_vm_contextp__->assertOnGet(1, 1)) {
        if (VL_UNLIKELY(((1U & (~ ((~ (IData)(vlSelfRef.__Vsampled_TOP__rst_n)) 
                                   | ((~ (IData)(vlSelfRef.__Vsampled_TOP__skid_buffer__DOT___Vpast_0_0)) 
                                      | ((IData)(vlSelfRef.__Vsampled_TOP__m_valid) 
                                         & ((IData)(vlSelfRef.__Vsampled_TOP__skid_buffer__DOT___Vpast_1_0) 
                                            == (IData)(vlSelfRef.__Vsampled_TOP__m_data)))))))))) {
            VL_WRITEF_NX("[%0t] %%Error: skid_buffer.sv:69: Assertion failed in %Nskid_buffer: skid_buffer dropped or mutated a stalled beat\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name());
            VL_STOP_MT("rtl/skid_buffer.sv", 69, "");
        }
    }
    vlSelfRef.skid_buffer__DOT___Vpast_1_0 = vlSelfRef.__Vsampled_TOP__m_data;
    vlSelfRef.skid_buffer__DOT___Vpast_0_0 = ((IData)(vlSelfRef.__Vsampled_TOP__rst_n) 
                                              & ((IData)(vlSelfRef.__Vsampled_TOP__m_valid) 
                                                 & (~ (IData)(vlSelfRef.__Vsampled_TOP__m_ready))));
    if ((1U & ((~ (IData)(vlSelfRef.skid_buffer__DOT__out_valid)) 
               | (IData)(vlSelfRef.m_ready)))) {
        vlSelfRef.skid_buffer__DOT__out_data = ((IData)(vlSelfRef.skid_buffer__DOT__skid_valid)
                                                 ? (IData)(vlSelfRef.skid_buffer__DOT__skid_data)
                                                 : (IData)(vlSelfRef.s_data));
    }
    vlSelfRef.m_data = vlSelfRef.skid_buffer__DOT__out_data;
    if (vlSelfRef.skid_buffer__DOT__accept) {
        vlSelfRef.skid_buffer__DOT__skid_data = vlSelfRef.s_data;
    }
}
