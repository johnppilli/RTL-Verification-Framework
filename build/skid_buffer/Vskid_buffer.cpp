// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vskid_buffer__pch.h"
#include "verilated_vcd_c.h"

//============================================================
// Constructors

Vskid_buffer::Vskid_buffer(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vskid_buffer__Syms(contextp(), _vcname__, this)}
    , clk{vlSymsp->TOP.clk}
    , rst_n{vlSymsp->TOP.rst_n}
    , s_valid{vlSymsp->TOP.s_valid}
    , s_ready{vlSymsp->TOP.s_ready}
    , s_data{vlSymsp->TOP.s_data}
    , m_valid{vlSymsp->TOP.m_valid}
    , m_ready{vlSymsp->TOP.m_ready}
    , m_data{vlSymsp->TOP.m_data}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
    contextp()->traceBaseModelCbAdd(
        [this](VerilatedTraceBaseC* tfp, int levels, int options) { traceBaseModel(tfp, levels, options); });
}

Vskid_buffer::Vskid_buffer(const char* _vcname__)
    : Vskid_buffer(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vskid_buffer::~Vskid_buffer() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vskid_buffer___024root___eval_debug_assertions(Vskid_buffer___024root* vlSelf);
#endif  // VL_DEBUG
void Vskid_buffer___024root___eval_static(Vskid_buffer___024root* vlSelf);
void Vskid_buffer___024root___eval_initial(Vskid_buffer___024root* vlSelf);
void Vskid_buffer___024root___eval_settle(Vskid_buffer___024root* vlSelf);
void Vskid_buffer___024root___eval(Vskid_buffer___024root* vlSelf);

void Vskid_buffer::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vskid_buffer::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vskid_buffer___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_activity = true;
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        vlSymsp->__Vm_didInit = true;
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vskid_buffer___024root___eval_static(&(vlSymsp->TOP));
        Vskid_buffer___024root___eval_initial(&(vlSymsp->TOP));
        Vskid_buffer___024root___eval_settle(&(vlSymsp->TOP));
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vskid_buffer___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vskid_buffer::eventsPending() { return false; }

uint64_t Vskid_buffer::nextTimeSlot() {
    VL_FATAL_MT(__FILE__, __LINE__, "", "No delays in the design");
    return 0;
}

//============================================================
// Utilities

const char* Vskid_buffer::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vskid_buffer___024root___eval_final(Vskid_buffer___024root* vlSelf);

VL_ATTR_COLD void Vskid_buffer::final() {
    Vskid_buffer___024root___eval_final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vskid_buffer::hierName() const { return vlSymsp->name(); }
const char* Vskid_buffer::modelName() const { return "Vskid_buffer"; }
unsigned Vskid_buffer::threads() const { return 1; }
void Vskid_buffer::prepareClone() const { contextp()->prepareClone(); }
void Vskid_buffer::atClone() const {
    contextp()->threadPoolpOnClone();
}
std::unique_ptr<VerilatedTraceConfig> Vskid_buffer::traceConfig() const {
    return std::unique_ptr<VerilatedTraceConfig>{new VerilatedTraceConfig{false, false, false}};
};

//============================================================
// Trace configuration

void Vskid_buffer___024root__trace_decl_types(VerilatedVcd* tracep);

void Vskid_buffer___024root__trace_init_top(Vskid_buffer___024root* vlSelf, VerilatedVcd* tracep);

VL_ATTR_COLD static void trace_init(void* voidSelf, VerilatedVcd* tracep, uint32_t code) {
    // Callback from tracep->open()
    Vskid_buffer___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vskid_buffer___024root*>(voidSelf);
    Vskid_buffer__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (!vlSymsp->_vm_contextp__->calcUnusedSigs()) {
        VL_FATAL_MT(__FILE__, __LINE__, __FILE__,
            "Turning on wave traces requires Verilated::traceEverOn(true) call before time 0.");
    }
    vlSymsp->__Vm_baseCode = code;
    tracep->pushPrefix(std::string{vlSymsp->name()}, VerilatedTracePrefixType::SCOPE_MODULE);
    Vskid_buffer___024root__trace_decl_types(tracep);
    Vskid_buffer___024root__trace_init_top(vlSelf, tracep);
    tracep->popPrefix();
}

VL_ATTR_COLD void Vskid_buffer___024root__trace_register(Vskid_buffer___024root* vlSelf, VerilatedVcd* tracep);

VL_ATTR_COLD void Vskid_buffer::traceBaseModel(VerilatedTraceBaseC* tfp, int levels, int options) {
    (void)levels; (void)options;
    VerilatedVcdC* const stfp = dynamic_cast<VerilatedVcdC*>(tfp);
    if (VL_UNLIKELY(!stfp)) {
        vl_fatal(__FILE__, __LINE__, __FILE__,"'Vskid_buffer::trace()' called on non-VerilatedVcdC object;"
            " use --trace-fst with VerilatedFst object, and --trace-vcd with VerilatedVcd object");
    }
    stfp->spTrace()->addModel(this);
    stfp->spTrace()->addInitCb(&trace_init, &(vlSymsp->TOP));
    Vskid_buffer___024root__trace_register(&(vlSymsp->TOP), stfp->spTrace());
}
