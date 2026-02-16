package services

import (
	"database/sql"
	"fmt"
	"net/http"
	"signalsync/backend/database"

	"github.com/gin-gonic/gin"
)

func ValidateLoginService(db *sql.DB, info *database.Login) (bool, string) {
	rows, errors := db.Query("SELECT password, api_key FROM login WHERE username = ?", info.Username)
	if errors != nil {
		return false, ""
	}
	fmt.Print(errors)
	if !rows.Next() {
		return false, ""
	}
	var password string
	var api_key string
	err := rows.Scan(&password, &api_key)
	fmt.Println(err)
	if err != nil {
		return false, ""
	}
	fmt.Println(password)
	fmt.Println(api_key)
	return password == (info.Password), api_key
}

func ValidateAPISessionService(db *sql.DB, info *database.SessionVerification) bool {
	rows, errors := db.Query("SELECT api_key FROM login WHERE username = ?", info.Username)
	if errors != nil {
		return false
	}
	if !rows.Next() {
		return false
	}
	var api_key string
	err := rows.Scan(&api_key)
	if err != nil {
		return false
	}
	return api_key == (info.Api_key)
}

func RegisterUserService(db *sql.DB, info *database.Login) bool {
	_, err := db.Exec("INSERT INTO login (username, password, api_key) VALUES (?, ?, ?)", info.Username, info.Password, info.Api_key)
	fmt.Print(err)
	return err == nil
}

func ValidateRequestBodyService[T any](req *T, c *gin.Context) bool {
	if err := c.ShouldBind(req); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		fmt.Println(err)
		return false
	}
	return true
}

func DeleteUserService(db *sql.DB, info *database.Login) bool {
	login_verification, _ := ValidateLoginService(db, info)
	if !login_verification {
		return false
	}
	_, err := db.Exec("DELETE FROM login WHERE username = ?", info.Username)
	fmt.Print(err)
	return err == nil
}
