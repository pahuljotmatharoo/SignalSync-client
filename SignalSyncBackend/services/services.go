package services

import (
	"database/sql"
	"fmt"
	"net/http"
	"signalsync/backend/database"

	"github.com/gin-gonic/gin"
)

func ValidateLoginService(db *sql.DB, info *database.Login) bool {
	rows, errors := db.Query("SELECT password FROM login WHERE username = ?", info.Username)
	if errors != nil {
		return false
	}
	fmt.Print(errors)
	error := rows.Scan(&info.Password) == nil
	fmt.Print(error)
	return !error
}

func RegisterUserService(db *sql.DB, info *database.Login) bool {
	_, err := db.Exec("INSERT INTO login (username, password) VALUES (?, ?)", info.Username, info.Password)
	fmt.Print(err)
	return err == nil
}

func ValidateLoginRequestBodyService(req *database.Login, c *gin.Context) bool {
	if err := c.ShouldBind(&req); err != nil {
		c.IndentedJSON(http.StatusBadRequest, nil)
		return false
	}
	return true
}

func DeleteUserService(db *sql.DB, info *database.Login) bool {
	login_verification := ValidateLoginService(db, info)
	if !login_verification {
		return false
	}
	_, err := db.Exec("DELETE FROM login WHERE username = ?", info.Username)
	fmt.Print(err)
	return err == nil
}
