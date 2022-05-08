# Software Design

## System 

### Context

@startuml C4_Elements
!include https://raw.githubusercontent.com/plantuml-stdlib/C4-PlantUML/master/C4_Container.puml

Person(customer, "Smart Home Customer", "A customer of the Smart Home System")
System(radiocloud_iot_system, "RadioCloud IoT System", "some description")

System_Ext(home_assistant, "Home Assistant", "HA")

Rel(customer, radiocloud_iot_system, "Uses")
Rel(customer, home_assistant, "Uses")
Rel_Neighbor(radiocloud_iot_system, home_assistant, "Exchanges Data", "REST API")

@enduml

### Container

@startuml C4_Elements
!include https://raw.githubusercontent.com/plantuml-stdlib/C4-PlantUML/master/C4_Container.puml

Person(customer, "Smart Home Customer", "A customer of the Smart Home System")

System_Ext(home_assistant, "Home Assistant", "HA")

System_Boundary(radiocloud_iot_boundary, "RadioCloud IoT System") {
    Container(radiocloud_iot_rf_node, "Generic IoT RF Node", "C, C++, Arduino, RFM69HCW", "Stores data from peripherial subcomponents and sends it periodically or as an event to the Gateway")

    Container(radiocloud_iot_rf_gateway, "IoT RF Gateway", "C++, Arduino, ESP32, RFM69HCW", "Collects data from Nodes, handles pairing, registration, authentication and forwards them to aggregation containers")

    Container(radiocloud_iot_api_app, "IoT API Application", "Java, Spring, Docker Container", "asd")
}


Rel(radiocloud_iot_rf_node, radiocloud_iot_rf_gateway, "Exchange Data", "RF Modulation (868MHz)")
Rel(radiocloud_iot_rf_gateway, radiocloud_iot_api_app, "RS232", "some description")

Rel(customer, radiocloud_iot_api_app, "Uses")
Rel(customer, home_assistant, "Uses")
Rel_Neighbor(radiocloud_iot_api_app, home_assistant, "Exchanges Data", "REST API")

@enduml

sdsdvsdv

## Message Exchange

Sequence diagram of basic message exchange is presented below. That's very common case when `node` sends `request` to the `receiver`. If message was processed successfully on `gateway` side, then sends successfull information. In other case notification about failure cause is passed.

@startuml 
activate Node

Node -> Gateway : request (msg-type)
activate Gateway

Gateway -> Gateway : process

alt successful case
    Gateway -> Node : success information
else some kind of failure
    Gateway -> Node : failure information
end
@enduml

Different message types (`msg-type`) are presented below:

### States

`State` is basic message which is send from `node` to the `gateway`. There are following kinds of messages:
- `BatteryState` - information about battery percentage value. This information is updated every `<batt-state-msg-period>` [h] hours value.
- `NodeState` - basic information about node's peripherials, e.g.: information from sensors. Period of sending particular `NodeState` is configured by `<node-state-msg-period>` [s] seconds.

### Actions

`Actions` are special messages exchanged between `node` and `gateway`. In general case `node` asks `gateway` for some data. There are following messages allowed:
- `configuration` - `gateway` sends configuration to the `node`,
- `update` - update firmware is triggered,
- `event` - `node` sends `event` to `gateway`, then `gateway` performs some action depending on received `event`. This kind of action is mostly triggered when external inperrupt is called. Configuration `<min-event-msg-period>` [s] must be configured.

Asking `gateway` for `configuration` is activated every time when `node` finished sending `state` to `gateway`. It is important that only `node` can initiate `action` message exchange. 

## Messaging API

### Base Message Format

```xml
<opening_char>
    <arg_1>
    <delim>
    <arg_2>
    <delim>
    <arg_n>
<closing_char>
```

Where:
- `<opening_char>` is `(`
- `<closing_char>` is `)`
- `<arg_1> ... <arg_n>` - arguments send by message,
- `<delim>` - separator between arguments, mostly `|` char.

Going further, message is grouped in following way:

```xml
<opening_char>
    <required_args>
    <optional_args>
<closing_char>
```

Where:
- `<required_args>` - parameters which are necessary to properly define message. Are created out of 2 parts:
    - `<message_direction>` - information for explicitly recignize message directions:
        - `request` = 1
        - `response` = 2
    - `<message_class>` - information about kind of message to inform parser how to parse particular data. Classes are described HERE!

- `<optional_args>` - mostly additional informations. Existende depends on particular message type.


### Message Request

`Request` is formatted in following way:

```xml
<opening_char>
    <message_direction>
    <delimeter>
    <message_class>
    <delimeter>
    <...additional_arguments...>
<closing_char>
```

`additional_arguments` depends on `message_class`.

#### Message Request Without Arguments

For example, very basic request:

```xml
(1|4)
```

Where:
- `<opening_char>` = `(`,
- `<message_direction>` = 1 (request),
- `<message_class>` = 5 (defined meanings HERE),
- `<closing_char>` = `)`,
- `delimeter` = `|`

#### Message Request With Arguments

Little bit advanced example of request:

```xml
(1|4|1,23|100|test)
```

Where:
- `<opening_char>` = `(`,
- `<message_direction>` = 1 (request),
- `<message_class>` = 4 (defined meanings HERE),
- `1,23|100|test` = arguments depending on `<message_class>`
- `<closing_char>` = `)`,
- `delimeter` = `|`

### Message Response

`Request` is formatted in following way:

```xml
<opening_char>
    <message_direction>
    <delimeter>
    <message_class>
    <delimeter>
    <status_args>
    <...additional_arguments...>
<closing_char>
```

Parameters mostly are the same like in `request`, but the only one difference is `<status_args>`, which is described in sections below.
`<status_args>` is created out of 2 subarguments:
- `<result>` - following values are allowed:
    - `1` - successfull result,
    - `0` - error result.
- `<error_description>

#### Successfull Message Response Without Parameters

When `request` was processed without errors, then `<status_args>` has 2 possible values: `0` when failed and `1` when success. For example:

```xml
(2|4|1)
```

#### Successfull Message Response With Parameters

```xml
(2|4|1|test|123,456|555|someData)
```

#### Error Message Response Without Parameters

That's the case when arguments are not available, because message was failed:

```xml
(2|4|0|)
```

## Messaging Library

### Message Buffer

### Message Interface

`Message Interface` consists of 3 elements presented below.

Basic of every message is `IMessage` interface which defines 2 operations:
- `parse` - when incoming message comes, then `string` representation in converted into `MessageModel<T>`.
- `build` - filled `MessageModel<T>` is converted into `string` representation.

@startuml 
interface IMessage
{
    + {abstract} bool parse(bool verify = true)
    + {abstract} bool build()
}
@enduml

### Message Direction

There are 2 general types of message:
- `request` - message which is received by endpoint,
- `response` - message which is send back to the requester.

It is necessary to send response every time. Then `requester` knows whether message was delivered and parsed properly. If there is no `response` is some specified `timeout` then `requester` can react approprietly. 

@startuml 
enum MessageDirection
{
    Request = 1,
    Response = 2
}
@enduml

### Message Model

`Message Model` abstract class is important part of every message

@startuml 
class MessageModel<T> 
{
    + T &model()
    + {abstract} bool parseModel()
    + {abstract} bool buildModel()
    - T model
}
@enduml


