package handlers

import (
	"database/sql"
	"net/http"
	dataBase "signalsync/backend/db"

	"github.com/gin-gonic/gin"
)

func ValidateLogin(db *sql.DB) gin.HandlerFunc {
	return func(c *gin.Context) {
		params := c.Params
		if len(params) < 2 {
			c.IndentedJSON(http.StatusBadRequest, nil)
			return
		}
		info := dataBase.Login{Username: c.Params[0].Value, Password: c.Params[1].Value}
		if(!ValidateLoginQuery(db, &info)) {
			c.IndentedJSON(http.StatusBadRequest, gin.H({"error", "Invalid Credentials"}))
		}
		else {
			c.IndentedJSON(http.StatusOK, gin.H({"success", "Login Successful"}))
		}
	}
}

func ValidateLoginQuery(db *sql.DB, info *dataBase.Login) bool {
	rows, errors := db.Query("SELECT password FROM login WHERE username = ?", info.Username)
		if errors != nil {
			return false
		}
		error := rows.Scan(&info.Password) == nil
		if error {
			return false
		}
		return true
}

func RegisterUserQuery(db *sql.DB, info *dataBase.Login) bool {
	result, err := db.Exec( "INSERT INTO users (username, password) VALUES (?, ?)", info.Username, info.Password)
	return err != nill
}

func RegisterUser(db *sql.DB) gin.HandlerFunc {
	return func(c *gin.Context) {
		params := c.Params
		if len(params) < 2 {
			c.IndentedJSON(http.StatusBadRequest, nil)
			return
		}
		info := dataBase.Login{Username: c.Params[0].Value, Password: c.Params[1].Value}
		if(!RegisterUserQuery(db, &info)) {
			c.IndentedJSON(http.StatusBadRequest, gin.H({"error", "Invalid Credentials"}))
		}
		else {
			c.IndentedJSON(http.StatusOK, gin.H({"success", "Register Successful"}))
		}
	}
}
