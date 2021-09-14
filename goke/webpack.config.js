const path = require("path");
const { isBundle } = require("typescript");

var config = {
  // TODO: Add common Configuration
  devtool: "eval-source-map",
  entry: "./src/client/ts/app.ts",
  module: {
    rules: [
      {
        test: /\.ts(x)?$/,
        use: "ts-loader",
        include: [path.resolve(__dirname, "src/client")],
        exclude: /node_modules/,
      },
      // {
      //   test: /\.scss$/,
      //   use: [MiniCssExtractPlugin.loader, "css-loader", "sass-loader"],
      // },
      // {
      //   test: /\.svg$/,
      //   use: "file-loader",
      // },
      // {
      //   test: /\.png$/,
      //   use: [
      //     {
      //       loader: "url-loader",
      //       options: {
      //         mimetype: "image/png",
      //       },
      //     },
      //   ],
      // },
    ],
  },
  resolve: {
    extensions: [".tsx", ".ts", ".js"],
  },
};

var clientConfig = Object.assign({}, config, {
  name: "client",
  // context: __dirname,
  // devtool: "eval-source-map",
  // entry: "./src/client/ts/app.ts",
  // module: {
  //   rules: [
  //     {
  //       test: /\.ts$/,
  //       use: "ts-loader",
  //       include: [path.resolve(__dirname, "src/client")],
  //     },
  //   ],
  // },
  output: {
    filename: "bundle.js",
    path: path.resolve(__dirname, "src/client/js"),
  },
  // resolve: {
  //   extensions: [".ts", ".js", ".json"],
  // },
});

var serverConfig = Object.assign({}, config, {
  name: "server",
  // context: __dirname,
  // devtool: "eval-source-map",
  // entry: "./src/client/ts/app.ts",
  // module: {
  //   rules: [
  //     {
  //       test: /\.ts$/,
  //       use: "ts-loader",
  //       include: [path.resolve(__dirname, "src/client")],
  //     },
  //   ],
  // },
  output: {
    filename: "bundle.js",
    path: path.resolve(__dirname, "src/server/static/js"),
  },
  // resolve: {
  //   extensions: [".ts", ".js", ".json"],
  // },
});

// Return Array of Configurations
module.exports = [clientConfig, serverConfig];

// module.exports = {
//   // context: __dirname,
//   devtool: "eval-source-map",
//   entry: "./src/client/ts/app.ts",
//   module: {
//     rules: [
//       {
//         test: /\.ts$/,
//         use: "ts-loader",
//         include: [path.resolve(__dirname, "src/client")],
//       },
//     ],
//   },
//   output: {
//     filename: "bundle.js",
//     path: path.resolve(__dirname, "src/client/js"),
//   },
//   resolve: {
//     extensions: [".ts", ".js", ".json"],
//   },
// };
