module.exports = {
  testEnvironment: "jsdom",
  transform: {
    "^.+\\.ts$": "ts-jest",
    "^.+\\.vue$": "@vue/vue3-jest"
  },
  moduleFileExtensions: ["ts", "js", "json", "vue"],
  roots: ["<rootDir>/src/test/javascript"]
};
