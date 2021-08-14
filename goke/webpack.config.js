const path = require("path");
const { isBundle } = require("typescript");

module.exports = {
  // context: __dirname,
  devtool: "eval-source-map",
  entry: "./src/client/static/ts/app.ts",
  module: {
    rules: [
      {
        test: /\.ts$/,
        use: "ts-loader",
        include: [path.resolve(__dirname, "src/client")],
      },
    ],
  },
  output: {
    filename: "bundle.js",
    path: path.resolve(__dirname, "src/server/static/js"),
  },
  resolve: {
    extensions: [".ts", ".js", ".json"],
  },
};
