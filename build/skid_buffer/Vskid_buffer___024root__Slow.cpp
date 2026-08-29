// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vskid_buffer.h for the primary calling header

#include "Vskid_buffer__pch.h"
#include "Vskid_buffer__Syms.h"
#include "Vskid_buffer___024root.h"

void Vskid_buffer___024root___ctor_var_reset(Vskid_buffer___024root* vlSelf);

Vskid_buffer___024root::Vskid_buffer___024root(Vskid_buffer__Syms* symsp, const char* v__name)
    : VerilatedModule{v__name}
    , vlSymsp{symsp}
 {
    // Reset structure values
    Vskid_buffer___024root___ctor_var_reset(this);
}

void Vskid_buffer___024root::__Vconfigure(bool first) {
    (void)first;  // Prevent unused variable warning
}

Vskid_buffer___024root::~Vskid_buffer___024root() {
}
