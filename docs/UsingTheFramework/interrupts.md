# Interrupt Guidelines

Most bare-metal embedded products require two modes of operation:

* Interrupt (or service) mode
* Non-interrupt (or user) mode.

The job of the code executed in interrupt mode is to respond to hardware events (interrupts) by performing minimal job of updating various status registers and schedule proper handling of event (if applicable) to be executed in non-interrupt mode.

In most projects the interrupt handlers are not prioritised, and the next hardware event (interrupt) won't be handled until the previously called interrupt handler returns, i.e., CPU is ready to return to non-interrupt mode.

To reduce latency, it is important for the interrupt handler is as minimal as possible.
