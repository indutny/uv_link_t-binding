'use strict';

const binding = require('bindings')('binding');

const net = require('net');

const server = net.createServer((c) => {
  c.end('hello');
}).listen(14000, () => {
  const s = net.connect(14000, () => {
    const source = new binding.Source(s._handle._externalUVStream);
    s._handle = null;
    s.destroy();

    const o = new binding.Observer();

    binding.chain(source.link, o.link);

    o.setStreaming(true);
    const phantom = new net.Socket({ handle: o });

    phantom.on('data', (buf) => {
      console.log(buf);
    });

    phantom.on('end', (buf) => {
      console.log('end');
      server.close();
    });
  });
});
