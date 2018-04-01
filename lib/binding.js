/*!
 * node-sass: lib/binding.js
 */

var errors = require('./errors');

/**
 * Require binding
 */
module.exports = function(ext) {
  console.log('1')
  if (!ext.hasBinary(ext.getBinaryPath())) {
    console.log('2')
    if (!ext.isSupportedEnvironment()) {
      console.log('3')
      throw new Error(errors.unsupportedEnvironment());
    } else {
      console.log('4')
      throw new Error(errors.missingBinary());
    }
  }
  console.log(ext.getBinaryPath())
  return require(ext.getBinaryPath());
};
