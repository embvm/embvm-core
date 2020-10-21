# Conceptual Architecture View

The Conceptual Architecture View of the Embedded Framework showcases the significant components and abstractions making up the framework. Connections and dependencies among the components are also shown. Interfaces and type information is not included within this view.

This view largely ignores the Software Layer, aside from showcasing that the Client Application depends on the Virtual Platform.

**Table of Contents:**

1. [Primary Presentation](#primary-presentation)
2. [Context Diagram](#context-diagram)
3. [Element Catalog](#element-catalog)
4. [Variability Guide](#variability-guide)
5. [Rationale](#rationale)
6. [Related Views](#related-views)

## Primary Presentation

<center>

| ![Conceptual Architecture](ConceptualModel.png) |
|:--:|
| *Figure 1. Conceptual Architecture for Embedded Framework.* |

</center>

## Context Diagram

The Conceptual Architecture View is a refinement of the [Layer View](../layer/layer_view.md). The Conceptual View shows the major components of the Platform, Processor, and Utility layers. Connections within the layers are explored in further detail than the [Layer View](../layer/layer_view.md) can show.

<center>

| ![Context for the Conceptual Architecture](../layer/LayerView.png) |
|:--:|
| *Figure 3. The Conceptual Architecture View is a refinement of the Platform, Processor, and Utility Layers.* |

</center>

## Element Catalog

The elements shown in this view are discussed in further detail within the [Layer View](../layer/layer_view.md). The Layer View elements have been been expanded in the following ways:
* The following elements represent new concepts contained within the Platform layer:
	* Event Queue
* The following elements represent new concepts contained within the Utilities layer:
	* Instance List
	* Interrupt Lock
	* Interrupt Queue
	* Dispatch Queue
	* Register
	* Static Function
	* Static Queue

Detailed component information is available for each element:

* Processor Layer
	* [Virtual Processor](../../components/core/virtual_processor.md)
	* [Virtual Processor Architecture](../../components/core/virtual_processor_architecture.md)
* Hardware Platform Layer
	- [Driver](../../components/core/driver.md)
	- [HAL Driver](../../components/core/hal_driver.md)
	- [Driver Registry](../../components/core/driver_registry.md)
	- [Register](../../components/core/register.md)
	- [Virtual Hardware Platform](../../components/core/virtual_hardware_platform.md)
* Platform Layer
	* [Boot Sequencer](../../components/core/boot_sequencer.md)
	* [C/C++ Runtime](../../components/core/c_cpp_runtime.md) 
	* [Event Center](../../components/core/event_center.md)
	- [Timer Manager](../../components/core/timer_manager.md)
	* [Virtual Platform](../../components/core/virtual_platform.md)
* Utilities Layer
	* [Dispatch Queue](../../components/utilities/dispatch_queue.md)
	* [Instance List](../../components/utilities/instance_list.md)
	* [Interrupt Lock](../../components/utilities/interrupt_lock.md)
	* [Interrupt Queue](../../components/utilities/interrupt_queue.md)
	* [Static Function](../../components/utilities/static_function.md)
	* [Static Queue](../../components/utilities/static_queue.md)

## Variability Guide

The variability options described in this view are a subset of the variability points in the [Layer View](../layer/layer_view.md).

The following variability options are available for elements shown in this view:

* Multiple client applications can be written to a given Virtual Platform interface
* New Virtual Platforms can be defined
	* Multiple Virtual Platforms can use a single Virtual Platform interface as long as interfaces and guarantees are satisfied
* Additional RTOS implementations may be added as long as they conform to the Virtual RTOS interfaces
* New Virtual Hardware Platforms can be defined
	* Multiple hardware platform implementations can use a single Virtual Hardware Platform interface as long as interfaces and guarantees are satisfied
* Additional processors may be added by implementing a new Virtual Processor
* Additional processor architectures can be added, as long as a new abstract interface for the architecture is defined
* Additional drivers may be added and used by the Virtual Hardware Platform and Virtual Processor
* Additional categories of drivers may be supported by defining a new abstract interface for that type
* Developers can override the default Boot Sequencer to suit the needs of their Virtual Platform

## Rationale

This view refines the concepts introduced in the [Layer View](../layer/layer_view.md) by showcasing the connections and dependencies between the participating elements.

This architecture focuses on the framework core components, and the Software Layer is largely excluded here. The Client Application represents higher-level software functionality within this view. The view shows that the Software Layer has a dependency on the Virtual Platform interfaces.

The connections between the elements are constrained per the layering requirements. As documented, the only violation of the layering rules is the Virtual Processor invoking the platform Boot Sequencer. This call is fixed in name.

## Related Views

This Conceptual Architecture View is a refinement of the [Layer View](../layer/layer_view.md).
