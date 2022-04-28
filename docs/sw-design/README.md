# Software Design

sdsdvsdv

## Messaging API

### Basic Message Exchange

Sequence diagram of basic message exchange is presented below. That's very common case when `requester` sends `request` to the `receiver`. If message was processed successfully on `receiver` side, then sends successfull information. In other case notification about failure cause is passed.

@startuml 
activate Requester

Requester -> Receiver : request
activate Receiver

Receiver -> Receiver : process

alt successful case
    Receiver -> Requester : success information
else some kind of failure
    Receiver -> Requester : failure information
end
@enduml

#### Base Message Format

```xml
<opening_char><arg_1><delim><arg_2><delim><arg_n><closing_char>
```

Where:
- `<opening_char>` is `(`
- `<closing_char>` is `)`
- `<arg_1> ... <arg_n>` - arguments send by message,
- `<delim>` - separator between arguments, mostly `|` char.

Going further, message is grouped in following way:

```xml
<opening_char><required_args><optional_args><closing_char>
```

Where:
- `<required_args>` - parameters which are necessary to properly define message. Are created out of 2 parts:
    - `<message_direction>` - information for explicitly recignize message directions:
        - `request` = 1
        - `response` = 2
    - `<message_class>` - information about kind of message to inform parser how to parse particular data. Classes are described HERE!

- `<optional_args>` - mostly additional informations. Existende depends on particular message type.


#### Message Request

`Request` is formatted in following way:

```xml
<opening_char><message_direction><delimeter><message_type><...additional_arguments...><closing_char>
```

For example, very basic request:

```xml
(1|4)
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


