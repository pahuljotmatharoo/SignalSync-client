package handlers

import (
	"database/sql"
	"fmt"
	"net/http"
	"signalsync/backend/database"

	"github.com/gin-gonic/gin"
)

func validateLoginRequestBody(req *database.Login, c *gin.Context) bool {
	if err := c.ShouldBind(&req); err != nil {
		c.IndentedJSON(http.StatusBadRequest, nil)
		return false
	}
	return true
}

func ValidateLogin(db *sql.DB) gin.HandlerFunc {
	return func(c *gin.Context) {
		var req database.Login

		if !validateLoginRequestBody(&req, c) {
			return
		}

		if !ValidateLoginQuery(db, &req) {
			c.IndentedJSON(http.StatusBadRequest, gin.H{"error": "Invalid Credentials"})
			return
		}

		c.IndentedJSON(http.StatusOK, gin.H{"success": "Login Successful"})

	}
}

func ValidateLoginQuery(db *sql.DB, info *database.Login) bool {
	rows, errors := db.Query("SELECT password FROM login WHERE username = ?", info.Username)
	if errors != nil {
		return false
	}
	error := rows.Scan(&info.Password) == nil
	return !error
}

func RegisterUserQuery(db *sql.DB, info *database.Login) bool {
	_, err := db.Exec("INSERT INTO users (username, password) VALUES (?, ?)", info.Username, info.Password)
	return err != nil
}

func RegisterUser(db *sql.DB) gin.HandlerFunc {
	return func(c *gin.Context) {
		var req database.Login

		if !validateLoginRequestBody(&req, c) {
			return
		}

		fmt.Println(req.Username)
		fmt.Println(req.Password)

		if !RegisterUserQuery(db, &req) {
			c.IndentedJSON(http.StatusBadRequest, gin.H{"error": "Invalid Credentials"})
			return
		}

		c.IndentedJSON(http.StatusOK, gin.H{"success": "Register Successful"})

	}
}
