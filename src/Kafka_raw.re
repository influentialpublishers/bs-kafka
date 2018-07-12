/** Node Buffer */
type buffer = Node.buffer;

/* HighLevelConsumer */
/* HighLevelProducer */
/* ProducerStream */
/* ConsumerGroup */
/* ConsumerGroupStream */
/* ConsumerStream */
/* Client */
[@bs.deriving abstract]
type zkOptions = {
  [@bs.optional]
  sessionTimeout: int,
  [@bs.optional]
  spinDelay: int,
  [@bs.optional]
  retries: int,
};
[@bs.deriving abstract]
type noAckBatchOptions = {
  [@bs.optional]
  noAckBatchSize: int,
  [@bs.optional]
  noAckBatchAge: int,
};
/* TODO fill out */
type sslOptions;
/* TODO */
type customPartitioner;
/* TODO */
type producerOptions;
module Client = {
  /** a zookeeper+kafka client */
  type t;

  /** construct a new zookeper/kafka client. arguments:

    connectionString: Zookeeper connection string

    clientId: This is a user-supplied identifier for the client application

    zkOptions: Zookeper options (see zkOptions type)

    noAckBatchOptions: when requireAcks is disabled on Producer side we can
    define the batch properties, 'noAckBatchSize' in bytes and 'noAckBatchAge'
    in milliseconds. The default value is { noAckBatchSize: null,
    noAckBatchAge: null } and it acts as if there was no batch

    sslOptions: not yet supported

    https://www.npmjs.com/package/kafka-node#client
    */
  [@bs.module "kafka-node"]
  [@bs.new]
  external make5 :
    (string, string, zkOptions, noAckBatchOptions, sslOptions) => t =
    "Client";

  /** construct a new zookeper/kafka client. arguments:

    connectionString: Zookeeper connection string

    clientId: This is a user-supplied identifier for the client application

    zkOptions: Zookeper options (see zkOptions type)

    noAckBatchOptions: when requireAcks is disabled on Producer side we can
    define the batch properties, 'noAckBatchSize' in bytes and 'noAckBatchAge'
    in milliseconds. The default value is { noAckBatchSize: null,
    noAckBatchAge: null } and it acts as if there was no batch

    https://www.npmjs.com/package/kafka-node#client
    */
  [@bs.module "kafka-node"]
  [@bs.new]
  external make4 : (string, string, zkOptions, noAckBatchOptions) => t =
    "Client";

  /** construct a new zookeper/kafka client. arguments:

    connectionString: Zookeeper connection string

    clientId: This is a user-supplied identifier for the client application

    zkOptions: Zookeper options (see zkOptions type)

    https://www.npmjs.com/package/kafka-node#client
    */
  [@bs.module "kafka-node"]
  [@bs.new]
  external make3 : (string, string, zkOptions) => t = "Client";

  /** construct a new zookeper/kafka client. arguments:

    connectionString: Zookeeper connection string

    clientId: This is a user-supplied identifier for the client application

    https://www.npmjs.com/package/kafka-node#client
    */
  [@bs.module "kafka-node"]
  [@bs.new]
  external make : (string, string) => t = "Client";

  /** close a client. arguments:

    t: the client to close

    f: a callback function to signal the closing of the client
    */
  [@bs.send]
  external close : (t, unit => unit) => unit = "close";
};
/* Producer */
module Producer = {
  /** a kafka producer */
  type t;

  /** construct a new kafka producer */ [@bs.module "kafka-node"] [@bs.new]
  external make : Client.t => t = "Producer";

  /** construct a new kafka producer */ [@bs.module "kafka-node"] [@bs.new]
  external make2 : (Client.t, producerOptions) => t = "Producer";

  /** construct a new kafka producer */ [@bs.module "kafka-node"] [@bs.new]
  external make3 : (Client.t, producerOptions, customPartitioner) => t =
    "Producer";

  /** the error type emitted with the 'error' event
   TODO fill in this type */
  type error;

  /** event handler TODO document */
  type errorEventHandler = error => unit;

  /** event handler TODO document TODO verify type */
  type readyEventHandler = unit => unit;

  /** add en event listener. supported events are: ready, error */ [@bs.send]
  external on :
    (
      t,
      [@bs.string] [ | `ready(readyEventHandler) | `error(errorEventHandler)]
    ) =>
    unit =
    "on";

  module ProduceRequest = {
    [@bs.deriving abstract]
    type t = {
      topic: string,
      messages: array(buffer),
      [@bs.optional]
      key: string,
      /** defaults to 0 */ [@bs.optional]
      parition: int,
      /** defaults to 0 = no compression; 1 = gzip; 2 = snappy */ [@bs.optional]
      attributes: int,
      /** defaults to now (only available with kafka v0.10 and KafkaClient only) */ [@bs.optional]
      timestamp: int,
    };
  };

  type sendErrorInner;
  /** error passed to your send completion callback TODO TYPEME */
  type sendError = Js.Nullable.t(sendErrorInner);

  type sendResultInner;
  /** successful result passed to your send completion callback TODO TYPEME */
  type sendResult = Js.Nullable.t(sendResultInner);
  /** Completion callback for async `send` method.
   TODO what types are err and data? README does not say */
  type sendCallback = (sendError, sendResultInner) => unit;

  /** Send messages to kafka */ [@bs.send] [@bs.send]
  external send : (t, array(ProduceRequest.t), sendCallback) => unit = "";
  /* TODO do createTopics method */
};

/* Consumer */
module Consumer = {
  /** a kafka consumer */
  type t;

  module Payload = {
    /** a structure used to describe which messages we want to receive */
    [@bs.deriving abstract]
    type t = {
      topic: string,
      /** defaults to 0 */ [@bs.optional]
      offset: int,
      /** defaults to 0 */ [@bs.optional]
      partition: int,
    };
  };

  module MakeOptions = {
    /** consumer make options */
    [@bs.deriving abstract]
    type t = {
      /** consumer group id, default `kafka-node-group` */ [@bs.optional]
      groupId: string,
      /** Auto commit config */ [@bs.optional]
      autoCommit: bool,
      /** Auto commit config */ [@bs.optional]
      autoCommitIntervalMs: int,
      /** The max wait time is the maximum amount of time in milliseconds to block waiting if insufficient data is available at the time the request is issued, default 100ms */ [@bs.optional]
      fetchMaxWaitMs: int,
      /** This is the minimum number of bytes of messages that must be available to give a response, default 1 byte */ [@bs.optional]
      fetchMinBytes: int,
      /** The maximum bytes to include in the message set for this partition. This helps bound the size of the response. */ [@bs.optional]
      fetchMaxBytes: int,
      /** If set true, consumer will fetch message from the given offset in the payloads */ [@bs.optional]
      fromOffset: bool,
      /** If set to 'buffer', values will be returned as raw buffer objects. TODO bs.string only valid string values */ [@bs.optional]
      encoding: string,
      /** Example value from README: 'utf8' */ [@bs.optional]
      keyEncoding: string,
    };
  };

  /** construct a new kafka consumer */ [@bs.module "kafka-node"] [@bs.new]
  external make3 : (Client.t, array(Payload.t), MakeOptions.t) => t =
    "Consumer";

  /** construct a new kafka consumer */ [@bs.module "kafka-node"] [@bs.new]
  external make : (Client.t, array(Payload.t)) => t = "Consumer";

  /** these are delivered to your error event handler TODO TYPEME */
  type errorEvent;
  /** event handler */
  type errorEventHandler = errorEvent => unit;

  /** passed to your offsetOutOfRangeHandler TODO TYPEME */
  type offsetOutOfRangeEvent;
  /** event handler */
  type offsetOutOfRangeHandler = offsetOutOfRangeEvent => unit;

  /** a type to obscure the dynamically typed 'value' property of the message
   passed to message handlers */
  type messageValue;

  /** passed to your message handler. TODO confirm this type */
  [@bs.deriving abstract]
  type message = {
    topic: string,
    value: messageValue,
    offset: int,
    partition: int,
    highWaterOffset: int,
    key: Js.Nullable.t(string),
  };

  /** obtain the 'value' property of the message if it's a buffer, otherwise
   throw an exception. to get a buffer, make sure you use encoding='buffer'
   in your consumer */
  let bufferValue: message => Node.buffer =
    message => message |> value |> Kafka_util_instanceof.bufferGetExn;

  /* event handler */
  type messageHandler = message => unit;

  [@bs.send]
  external on :
    (
      t,
      [@bs.string] [
        | `error(errorEventHandler)
        | `offsetOutOfRange(offsetOutOfRangeHandler)
        | `message(messageHandler)
      ]
    ) =>
    unit =
    "";
};
/* KafkaClient */
/** Provides the deriving abstract type `t` */
module OffsetRequest = {
  /** a structure used to describe which topics+partitions we want to operate
   with. used by Offset.fetch */
  [@bs.deriving abstract]
  type t = {
    topic: string,
    [@bs.optional]
    partition: int,
    /** Used to ask for all messages before a certain time (ms), default Date.now(),
     Specify -1 to receive the latest offsets and -2 to receive the earliest available offset.
     */
    [@bs.optional]
    time: int,
    /** default 1 */
    [@bs.optional]
    maxNum: int
  };
};

module OffsetCommitRequest = {
  /** a structured used to describe which topic+partitions we want to operate
   * with, and what offsets we want to commit for those. used with
   * Offset.commit */
  [@bs.deriving abstract]
  type t = {
    topic: string,
    /** default 0 */
    [@bs.optional]
    partition: int,
    offset: int,
    /** default "m" */
    [@bs.optional]
    metadata: string,
  };
};

/* Offset */
module Offset = {
  /** a kafka offset... thing TODO */
  type t;

  /** the type of the callback you supply to fetch TODO define */
  type fetchCallback;

  /** fetch the available offset of a specific topic-partition */
  [@bs.send] external fetch : (t, array(OffsetRequest.t), fetchCallback) => unit = "";

  /** the type of the callback you supply to commit TODO define */
  type commitCallback;

  /** commit offsets to zookeeper WARNING: commits are made to zookeeper and is
   only compatible with `HighLevelConsumer` and will NOT with the new
   `ConsumerGroup`. arguments:
   0: string groupId identifies the consumer group whose offsets you wish to manipulate
   1: array of offsets you wish to commit
   2: completion callback */
  [@bs.send]
  external commit :
    (t, string, array(OffsetCommitRequest.t), commitCallback) => unit = "";

  /** register event handlers */
  [@bs.send]
  external on :
    (t, [@bs.string] [ | `ready(unit => unit) | `connect(unit => unit)]) =>
    unit =
    "";

  /* TODO fetchCommits */
  /* TODO fetchLatestOffsets */
  /* TODO fetchEarliestOffsets */
};

/* Admin */
/* KeyedMessage */
/* DefaultPartitioner */
/* CyclicPartitioner */
/* RandomPartitioner */
/* KeyedPartitioner */
/* CustomPartitioner */
