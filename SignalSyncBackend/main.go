package main

import (
	"database/sql"
	"os"

	"signalsync/backend/database"

	"signalsync/backend/controllers"

	"github.com/gin-gonic/gin"

	"github.com/joho/godotenv"
)

//packages are like namespaces (hold set of functions/types)

//module is basically your project

//Random info:
// char = byte

// data structures:

// Arrays
// [1] is fixed size array (x := [1]int{1})
// [] with value is dynamic array (vector) (x := []int{1,2,3}) - This is called a Slice (don't copy, they are by reference copies)
// 						but usually, x := make([]int, size) - Do same for maps

// for i := range arr  this is just range based loop, creates copies per element, no way to get by reference
// for i := 0; i < len(arr); i++ , just a normal for loop
// for i, v := range arr , this is both range and index based, v is value and i is index

// len(arr) for the size
// arr = append(arr, value)
// probably other methods like this, remember since its not oop, all methods require the pointer itself

// Maps
// m := make(map[key]value)
// map[key]value {} is a hash map
// just do map[key] to see if it exists, 0 => it doesn't exist
// val, ok := m["apple"] (where ok is a bool &  val is the value)
// delete(map, key) to remove it

// Middleware: code that runs in the middle of a request’s journey
//			   runs after we get request, but before we send response
// ex: Client -> Middleware 1 (check login) -> Middleware 2 (check authentication) -> Middleware 3 (validate the request) -> Send Response

func defineEndpoints(router *gin.Engine, db *sql.DB) {
	router.POST("/login", controllers.ValidateLogin(db))
	router.POST("/register", controllers.RegisterUser(db))
	router.POST("/delete", controllers.DeleteUser(db))
	router.POST("/validateApiKey", controllers.ValidateAPIKey(db))
	router.POST("/changePassed", controllers.UpdatePasswordController(db))
}

func main() {
	godotenv.Load()
	router := gin.Default()
	db := database.InitDataBase()
	defineEndpoints(router, db)
	router.Run(os.Getenv("HTTP_ADDRESS"))
}
